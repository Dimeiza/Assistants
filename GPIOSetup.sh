#!/bin/bash

CURRENT_DIR="$(pwd)"
INSTALL_BASE=${INSTALL_BASE:-"$CURRENT_DIR"}
THIRD_PARTY_PATH="$CURRENT_DIR/third-party"

cd "$THIRD_PARTY_PATH"

git clone git://git.drogon.net/wiringPi
cd ~/wiringPi
./build

