#!/bin/bash

CURRENT_DIR="$(pwd)"
INSTALL_BASE=${INSTALL_BASE:-"$CURRENT_DIR"}
THIRD_PARTY_PATH="$CURRENT_DIR/third-party"

mkdir -p "$THIRD_PARTY_PATH"
cd "$THIRD_PARTY_PATH"
git clone https://github.com/respeaker/seeed-voicecard.git
cd seeed-voicecard
git checkout 8cce4e8ffa77e1e2b89812e5e2ccf6cfbc1086cf
sudo ./install.sh --compat-kernel

cd ..

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
