import snowboydecoder
import sys
import signal
import subprocess
from time import  sleep
import sys
import posix_ipc
import json
import time
import shutil
import os
import requests
import yaml

from mic_hat_4 import pixels
from mic_hat_4 import alexa_led_pattern
from mic_hat_4 import google_home_led_pattern

from MMMApi import MMMApi

interrupted = False
MMMApiInstance = MMMApi()

ALEXA = 0
GOOGLE_ASSISTANT = 1

def signal_handler(signal, frame):

    assistantsControl_mq.close()
    posix_ipc.unlink_message_queue("/AssistantsControlQueue")
    global interrupted
    interrupted = True


def interrupt_callback():

    global interrupted
    return interrupted

def alexa_callback():

    pixels.pixels.pattern = alexa_led_pattern.AlexaLedPattern(show=pixels.pixels.show)
    print('alexa detected!')
    communicateAssistant(led = pixels.pixels,messageQueue = alexa_mq,assistant_no = ALEXA)
    print('alexa finished')
    
def google_callback():


    pixels.pixels.pattern = google_home_led_pattern.GoogleHomeLedPattern(show=pixels.pixels.show)

    print('google detected!')
    communicateAssistant(led = pixels.pixels,messageQueue = google_mq,assistant_no = GOOGLE_ASSISTANT)
    print('google finished')
    
def communicateAssistant(led,messageQueue,assistant_no):

    led.wakeup()
    MMMApiInstance.wakeup(assistant_no)

    # remove a messages of queue if have a message before Assistantcotrol sends wakeup to Assistant
    # (status synchronization)
    if assistantsControl_mq.current_messages != 0:
        # print('assistantsControl_mq is not empty')
        while assistantsControl_mq.current_messages != 0:
            msg = assistantsControl_mq.receive(timeout=3)
            # print(msg)

    print('wakeup')
    messageQueue.send('wakeup')

    while True:
        try:
            # receive with timeout. 
            # If Assistant already finished. Assistant controller receives no message from Assistant and finishes after timeout.
            msg = assistantsControl_mq.receive(timeout=15)
            if msg[0] == b'finish':
                MMMApiInstance.finish(assistant_no)
                break
            elif msg[0] == b'speak':
                MMMApiInstance.off()
                MMMApiInstance.speak(assistant_no)
                led.speak()
            elif msg[0] == b'think':
                MMMApiInstance.think(assistant_no)
                led.think()
        except posix_ipc.BusyError:
            break

    sleep(0.5)
    led.off()

sensitivity = []
callbacks = []
models = []

if __name__=='__main__':

    # capture SIGINT signal, e.g., Ctrl+C
    signal.signal(signal.SIGINT, signal_handler)

    with open('AssistantControl.yaml', 'r') as yml:
        config = yaml.load(yml)

    for Assistant in config['Assistants'].values():
        sensitivity.append(Assistant['Sensitibity'])
        models.append(Assistant['Model'])
        callbacks.append(globals()[Assistant['Callback']])

    detector = snowboydecoder.HotwordDetector(models, sensitivity=sensitivity)
    print('Listening... Press Ctrl+C to exit')

    google_mq = posix_ipc.MessageQueue("/GoogleAssistantQueue", posix_ipc.O_CREAT)
    alexa_mq = posix_ipc.MessageQueue("/AlexaQueue", posix_ipc.O_CREAT)
    assistantsControl_mq = posix_ipc.MessageQueue("/AssistantsControlQueue",posix_ipc.O_CREAT,read=True)

    detector.start(detected_callback=callbacks,
                interrupt_check=interrupt_callback,
               sleep_time=0.03)

    detector.terminate()