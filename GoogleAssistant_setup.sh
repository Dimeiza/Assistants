#!/bin/bash

CURRENT_DIR="$(pwd)"
INSTALL_BASE=${INSTALL_BASE:-"$CURRENT_DIR"}

echo "Enter informations for Google Assistant SDK."
read -r -p "Your credentiail JSON filename:" credentialfilename
echo ""
read -r -p "Your Google Cloud Console Project-Id: " projectid
echo ""
read -r -p "Product Name of your device:" productname
echo ""

modelid=$projectid-$(date +%Y%m%d%H%M%S )

# for google Assistant
sudo apt-get -y install python3-dev python3-venv # Use python3.4-venv if the package cannot be found.
sudo apt-get -y install portaudio19-dev libffi-dev libssl-dev

python3 -m venv env
env/bin/python -m pip install --upgrade pip setuptools
source env/bin/activate

python -m pip install --upgrade google-assistant-library

python -m pip install --upgrade google-assistant-sdk[samples]

python -m pip install --upgrade google-auth-oauthlib[tool]

google-oauthlib-tool --client-secrets "$CURRENT_DIR/$credentialfilename" --scope https://www.googleapis.com/auth/assistant-sdk-prototype --save --headless
googlesamples-assistant-devicetool register-model --manufacturer "Raspberry Pi Foundation" --product-name $productname --type LIGHT --model $modelid

echo "Testing the installed google assistant. Make a note of the generated Device-Id"

cat << EOF > "startGoogleAssistant.sh"
#!/bin/bash
source env/bin/activate
googlesamples-assistant-hotword --project_id $projectid --device_model_id $modelid
EOF

chmod +x startGoogleAssistant.sh

echo "================================================================"
echo 'Install finished.'
echo 'You can run Google Assistant hotword sample following command.'
echo '"./startGoogleAssisntat.sh"'
echo "================================================================"


