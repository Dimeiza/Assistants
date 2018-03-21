#!/bin/bash

set -o errexit

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root (use sudo)" 1>&2
   exit 1
fi

cp ./ServiceScript/Alexa-SampleApp.service /lib/systemd/system/Alexa-SampleApp.service
systemctl enable Alexa-SampleApp.service
systemctl start Alexa-SampleApp.service

cp ./ServiceScript/GoogleAssistant-hotword.service /lib/systemd/system/GoogleAssistant-hotword.service
systemctl enable GoogleAssistant-hotword.service
systemctl start GoogleAssistant-hotword.service

cp ./ServiceScript/AssistantControl.service /lib/systemd/system/AssistantControl.service
systemctl enable AssistantControl.service
systemctl start AssistantControl.service


