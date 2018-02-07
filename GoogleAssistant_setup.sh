#!/bin/bash

CURRENT_DIR="$(pwd)"
INSTALL_BASE=${INSTALL_BASE:-"$CURRENT_DIR"}

# for google Assistant
sudo apt-get -y install python3-dev python3-venv # Use python3.4-venv if the package cannot be found.
sudo apt-get -y install portaudio19-dev libffi-dev libssl-dev

python3 -m venv env
env/bin/python -m pip install --upgrade pip setuptools
source env/bin/activate

python -m pip install --upgrade google-assistant-library

python -m pip install --upgrade google-assistant-sdk[samples]

python -m pip install --upgrade google-auth-oauthlib[tool]


