#!/bin/bash

CURRENT_DIR="$(pwd)"
INSTALL_BASE=${INSTALL_BASE:-"$CURRENT_DIR"}
THIRD_PARTY_PATH="$CURRENT_DIR/third-party"

cd "$THIRD_PARTY_PATH"
git clone https://github.com/respeaker/seeed-voicecard.git
cd seeed-voicecard
sudo ./install.sh 4mic

cd ..
git clone https://github.com/respeaker/4mics_hat

git clone https://github.com/Dimeiza/ReSpeakerLED

cd ReSpeakerLED
gcc -o ReSpeakerLED ReSpeakerLED.c
cd ..

echo "=========================================="
echo 'Install finished.'
echo 'And you have to reboot to apply update.'
echo 'If you use LEDs, you need to be enable SPI.'
echo '1. Run "sudo raspi-config".'
echo '2. Select "5 Interfacing Options".'
echo '3. Select "P4 SPI".'
echo '4. Select "Yes".'
echo "=========================================="
