#!/bin/bash

echo "Enter informations for Alexa"
read -r -p "Your Client ID:" clientid
echo ""
read -r -p "Your Client Secret:" clientsecret
echo ""
read -r -p "your product id:" productid
echo ""

serialno=$productid-$(date +%Y%m%d%H%M%S )

cat << EOF > "config.txt"
CLIENT_SECRET="$clientsecret"
#NOTE: The Device Serial Number can be any unique number
DEVICE_SERIAL_NUMBER="$serialno"
CLIENT_ID="$clientid"
PRODUCT_ID="$productid"
EOF

wget https://raw.githubusercontent.com/Dimeiza/avs-device-sdk/master/tools/RaspberryPi/setup.sh
chmod 700 ./setup.sh

./setup.sh config.txt
