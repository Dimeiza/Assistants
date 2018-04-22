# AssistantControl(Tentative name)

Tools to assist when run Alexa and Google Assistant at the same time on Raspberry Pi(with Respeaker 2mic/4mic).

## Notice

This tool is an eternal PoC(Proof of Concept). You can only use this tool at your own risk. 
And I strongly recommended that this tool only use a power user.
If you are a newbie of raspberry pi and linux, I recommend that you don't use this tool, and you use both assistants only according to official procedures.

In principle, I don't hope to receive a question how to execute Assistants in your environment. But if you find a point of improvement, you can share it in issue in this repository. I can't promise to improve, but I will read your issue.

## Description

This tool/scripts features are:

* Wakeup voice control that independent from both assistants. 
    * This function allow you use wakeup voice control on Raspberry Pi Zero.
    * You can use original wake word instead of default.
* Control of Respeaker's LEDs synchronized with the status of both assistants.

* Install/Uninstall both assistant and above function as a service.

## Demo

Rapsberry Pi Zero + ReSpeaker 2-Mics Pi HAT.

https://www.youtube.com/watch?v=P66bfCi45wE

Raspberry Pi 3 + ReSpeaker 4-Mic Array

https://www.youtube.com/watch?v=WsatqjlLYls

## VS. 

If you only want to run both assistant simultaneously and you don't have ReSpeaker, you can use [AssistantPi](https://github.com/xtools-at/AssistantPi) instead of this tool.

## Requirement

You need:
1. A Raspberry Pi 3 and SD card with a flesh install of Raspbian Stretch

2. Audio peripherals:
    * external speaker with 3.5mm Jack

3. ReSpeaker
    * [ReSpeaker 2-Mics Pi HAT](https://www.seeedstudio.com/ReSpeaker-2-Mics-Pi-HAT-p-2874.html) or

    * [ReSpeaker 4-Mic Array for Raspberry Pi](https://www.seeedstudio.com/ReSpeaker-4-Mic-Array-for-Raspberry-Pi-p-2941.html)

4. USB Sound Output(Iecommended: if you use Respeaker 4-Mic).
    * [For example](https://www.amazon.co.uk/TechRise-External-Aluminum-Headphone-Microphone/dp/B0789CN8CT)
    * I seem it is stabler than builtin(bcm2835) sound output.

## Install

### Alexa setup

1. Boot with fresh Raspbian Stretch.
2. Clone this repository.

```
git clone https://github.com/Dimeiza/Assistants.git
```

3. Complete Alexa developer registration.
    * You need these information to register Raspberry Pi as Alexa Device.
        * Client ID
        * Product ID
    * I recommend that you refer [avs-device-sdk installation guide](https://github.com/alexa/avs-device-sdk/wiki/Raspberry-Pi-Quick-Start-Guide-with-Script) carefully.
    * If you had used AVS Device SDK before 1.6, see https://github.com/alexa/avs-device-sdk/wiki/Code-Based-Linking----Configuration-Update-Guide. Procedure to Prepare Client ID is changed.

4. Into Assistants folder, and run this command.

```
cd Assisntants
```

5. Get template of config.txt

```
wget https://raw.githubusercontent.com/alexa/avs-device-sdk/master/tools/Install/config.txt
```

6. Update config.txt with the Client ID and Product ID for your registered product and save.

7. Run the setup script with your configuration as an argument:

```
bash setup.sh config.txt
```

8. Wait to finish script.

### Google Assistant setup

1. Complete Google Assistant registration.
    * [Configure a Developer Project and Account Settings]
(https://developers.google.com/assistant/sdk/guides/library/python/embed/config-dev-project-and-account).
    * [Register the Device Model]
(https://developers.google.com/assistant/sdk/guides/library/python/embed/register-device#download-credentials)

    * You need these information.
        * Credential JSON file.
        * Your project name.
        * Product name.

2. Run this command.

```
bash GoogleAssistant.sh
```

3. Follow the prompt instructions to input registered information.

```
Your credentiail JSON filename:
Your Google Cloud Console Project-Id:
Product Name of your device:
```

4. Wait to finish script.

### Respeaker setup

1. Run this command according to your device.

ReSpeaker 2-Mics Pi HAT
```
bash Respeaker2Mic.sh
```

ReSpeaker 4-Mic Array for Raspberry Pi
```
bash Respeaker4Mic.sh
```

2. After script finished, configure SPI to be enable and reboot.

```
1. Run "sudo raspi-config".
2. Select "5 Interfacing Options".
3. Select "P4 SPI".
4. Select "Yes".
```

3. Reboot.

### AsssitantControl setup

1. Run This command.

```
setupAssistantControl.sh
```

2. Modify AssistantControl/AssistantControl.py as following to match your ReSpeaker.

* ReSpeaker 2-Mic Pi Hat

``` 
from mic_hat import pixels
from mic_hat import alexa_led_pattern
from mic_hat import google_home_led_pattern
```

* ReSpeaker 4-Mic Array for Raspberry Pi

``` 
from mic_hat_4 import pixels
from mic_hat_4 import alexa_led_pattern
from mic_hat_4 import google_home_led_pattern
```

## Usage

At first, I recommend that you use this tool with terminals.

1. Open Terminal and run these commands.

```
cd Assistants
./startsample.sh
```

After several seconds, Alexa is booted.
This Alexa can control with keyboard. If you needed, you can change a language Alexa uses.

2. Open 2nd Terminal and these commands.

```
cd Assistants
./startGoogleAssistants.sh
```

After several seconds, Google Assistant is booted. 
But this Google Assistant can't control standalone.
If you need a language Google Assistant uses, You can change it with using Google Assistant App with your smartphone.


3. Open 3rd Terminal and these commands.

```
cd Assistants
./startAssistantControl.sh
```

4. After several seconds, message stop.Then, say "Alexa" to raspberry pi. If LEDs light up, tell Alexa your instruction. After response of Alexa, LEDs light down.
    * (In this version, first several responses to "Alexa" may not work well.)

5. Next, say "Snowboy" to raspberry pi. If LEDs light up, tell Google Assistant your instruction. After response of Google Assistant, LEDs light down.
    * The default wake word of the Google Assistant for this tool is "Snowboy". According to the procedure described below, you can change it.

## wake word changing procedure

1. Make wake word

This tool uses [Snowboy](https://snowboy.kitt.ai). You can make original wake word in Snowboy official site.
Read the official Snowboy website for how to make wakewords.

If you make a wake word, You can download wake word file(.pmdl)

2. set wake word

wake word setting of this tool is in startAssistantControl.sh.

```
python AssistantControl.py resources/alexa.umdl resources/snowboy.umdl
```

wake word files are set as arguments of AssistantControl.py.
First argument is wake word of Alexa, and second argument is wake word of Google Assistant.
You can update these arguments to change both wake word.

For example, you want to change GoogleAssistant wake word and you already make wake word file as "HeyGoogle.pmdl".

You should copy "HeyGoogle.pmdl" to Assitant/AssistantControl/resources.
And You should change startAssistantControl.sh as following.

```
python AssistantControl.py resources/alexa.umdl resources/HeyGoogle.pmdl
```

Restart startAssistantControl.sh and try to say new wake word.

## Install as a service / Uninstall

If you success that you run this tool with terminals, you can install this tool as a service.

```
sudo bash InstallService.sh
```

After finishing script, Reboot.

If you want to uninstall service, run following.
```
sudo bash UninstallService.sh
```

## Known Issue

### Issue of Google Assistant  

There may be noise on Google Assistant's response when you use ReSpeaker 4-Mic + Raspberry Pi 3 audio output(without USB soundcard)

In this case, I recommend that you use PulseAudio with USB soundcard.

If you do so, check sound configuration before you run both assistants.

#### SoundDevice Check

1. On Raspbian Desktop, Open menu and choice "Sound & Video" -> "PulseAudio Volume Control".

1. Click "Configuration" Tab, and check that "seeed-4mic-voicecard" profile is "Multichannel input".

1. If you use USB Sound card, check "bcm2835 ALSA" profile is "Off" and "USB Audio Device" profile is "Analog Stereo output".

### Unsynchronized status between AssistantControl and both assistant

It may occur that a status of AssistantControl unsyncronize with Alexa or Google Assistant.
(For example, Alexa finished speaking but AssistantControl may not sense next wakeword)

In this case, this problem may be solved if you execute this command.

```
sudo rm /dev/mqueue/*
```

## dependency 

This tool depends these software.

* [AVS Device SDK](https://github.com/alexa/avs-device-sdk)
* [Google Assistant SDK](https://developers.google.com/assistant/sdk/)
* [PulseAudio](https://www.freedesktop.org/wiki/Software/PulseAudio/)
* [Snowboy](https://snowboy.kitt.ai/)
* Respeaker(GitHub)
    * [seeed-voicecard](https://github.com/respeaker/seeed-voicecard)
    * [2 Mic Array for Raspberry Pi](https://github.com/respeaker/mic_hat)
    * [4 Mics HAT for Raspberry Pi](https://github.com/respeaker/4mics_hat)


## Author

[dimeiza](https://github.com/Dimeiza)
