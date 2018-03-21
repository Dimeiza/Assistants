import snowboydecoder
import sys
import signal
import subprocess
from time import  sleep
import sys
import posix_ipc
import json
import time

from mic_hat_4 import pixels
from mic_hat_4 import alexa_led_pattern
from mic_hat_4 import google_home_led_pattern

interrupted = False

def signal_handler(signal, frame):
    global interrupted
    interrupted = True


def interrupt_callback():
    global interrupted
    return interrupted

def alexa_callback():

    pixels.pixels.pattern = alexa_led_pattern.AlexaLedPattern(show=pixels.pixels.show)
    print('alexa detected!')
    communicateAssistant(led = pixels.pixels,messageQueue = alexa_mq)
    print('alexa finished')
    
def google_callback():


    pixels.pixels.pattern = google_home_led_pattern.GoogleHomeLedPattern(show=pixels.pixels.show)

    print('google detected!')
    communicateAssistant(led = pixels.pixels,messageQueue = google_mq)
    print('google finished')
    
def communicateAssistant(led,messageQueue):

    led.wakeup()
    messageQueue.send('wakeup')
    
    while True:
        msg = messageQueue.receive()
        if msg[0] == b'finish':
            break
        elif msg[0] == b'speak':
            led.speak()
        elif msg[0] == b'think':
            led.think()
    sleep(0.5)
    led.off()



if len(sys.argv) != 3:
    print("Error: need to specify 2 model names")
    print("Usage: python demo.py 1st.model 2nd.model")
    sys.exit(-1)

models = sys.argv[1:]

# capture SIGINT signal, e.g., Ctrl+C
signal.signal(signal.SIGINT, signal_handler)

sensitivity = [0.4,0.5]
detector = snowboydecoder.HotwordDetector(models, sensitivity=sensitivity)
callbacks = [alexa_callback, google_callback]
print('Listening... Press Ctrl+C to exit')

google_mq = posix_ipc.MessageQueue("/google_assistant_queue", posix_ipc.O_CREAT)
alexa_mq = posix_ipc.MessageQueue("/alexa_queue", posix_ipc.O_CREAT)

# main loop
detector.start(detected_callback=callbacks,
               interrupt_check=interrupt_callback,
               sleep_time=0.03)

detector.terminate()
posix_ipc.unlink_message_queue("/google_assistant_queue")
posix_ipc.unlink_message_queue("/alexa_queue")
