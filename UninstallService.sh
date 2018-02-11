#!/bin/bash

systemctl stop Alexa-SampleApp.service
systemctl disable Alexa-SampleApp.service
rm /lib/systemd/system/Alexa-SampleApp.service

systemctl stop GoogleAssistant-hotword.service
systemctl disable GoogleAssistant-hotword.service
rm /lib/systemd/system/GoogleAssistant-hotword.service

