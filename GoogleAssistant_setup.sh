#!/bin/bash

CURRENT_DIR="$(pwd)"
INSTALL_BASE=${INSTALL_BASE:-"$CURRENT_DIR"}

echo "Enter informations for Google Assistant SDK."
read -r -p "Your credentiail JSON filename:" credentialfilename
echo ""
read -r -p "Your Google Cloud Console Project-Id: " projectid
echo ""
read -r -p "Device Model ID of your device:" modelid
echo ""

# for google Assistant
sudo apt-get -y install python3-dev python3-venv # Use python3.4-venv if the package cannot be found.
sudo apt-get -y install portaudio19-dev libffi-dev libssl-dev

python3 -m venv env
env/bin/python -m pip install --upgrade pip setuptools wheel
source env/bin/activate

python -m pip install --upgrade google-assistant-library

python -m pip install --upgrade google-assistant-sdk[samples]

python -m pip install --upgrade google-auth-oauthlib[tool]

google-oauthlib-tool --client-secrets "$CURRENT_DIR/$credentialfilename" --scope https://www.googleapis.com/auth/assistant-sdk-prototype --save --headless

python -m pip install posix_ipc

cp patch/GoogleAssistant/posixqueue.py env/lib/python3.5/site-packages/googlesamples/assistant/grpc
cp patch/GoogleAssistant/audio_helpers_for_queue.py env/lib/python3.5/site-packages/googlesamples/assistant/grpc
cp patch/GoogleAssistant/googlesamples-assistant-posixqueue env/bin/
chmod 755 env/bin/googlesamples-assistant-posixqueue

cat << EOF > "startGoogleAssistant.sh"
#!/bin/bash
source env/bin/activate
googlesamples-assistant-posixqueue --project-id $projectid --device-model-id $modelid
EOF

chmod +x startGoogleAssistant.sh

echo "================================================================"
echo 'Install finished.'
echo 'You can run Google Assistant hotword sample following command.'
echo '"./startGoogleAssisntant.sh"'
echo "================================================================"


