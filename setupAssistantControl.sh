#!/bin/bash

source env/bin/activate

cd AssistantControl

# setup snowboy
git clone https://github.com/Kitt-AI/snowboy.git
sudo apt-get -y install libatlas-base-dev

sudo apt-get -y  install swig3.0 python-pyaudio python3-pyaudio sox swig
pip install pyaudio spidev posix_ipc gpiozero numpy

cd snowboy/swig/Python3
make
cd ../../../

cp snowboy/examples/Python3/snowboydetect.py .
cp snowboy/examples/Python3/snowboydecoder.py .
cp snowboy/examples/Python3/_snowboydetect.so .
cp snowboy/examples/Python3/snowboy-1.2.0b1-py2-none-any.whl .

sed -i -e 's/from . import snowboydetect/import snowboydetect/' snowboydecoder.py

mkdir resources
cp snowboy/resources/common.res resources/
cp snowboy/resources/alexa/alexa-avs-sample-app/alexa.umdl resources/
cp snowboy/resources/models/snowboy.umdl resources/

# setup Respeaker LED code

git clone https://github.com/respeaker/mic_hat mic_hat
git clone https://github.com/respeaker/4mics_hat mic_hat_4

sed -i -e 's/import apa102/from . import apa102/' mic_hat/pixels.py
sed -i -e 's/self.basis/self.pattern/' mic_hat/pixels.py
sed -i -e 's/self.write/self.show/' mic_hat/pixels.py
sed -i -e 's/def write/def show/' mic_hat/pixels.py

touch mic_hat/__init__.py

mv alexa_led_pattern.py mic_hat/
mv google_home_led_pattern.py mic_hat/

sed -i -e 's/import apa102/from . import apa102/' mic_hat_4/pixels.py
sed -i -e 's/from alexa_led_pattern import AlexaLedPattern/from .alexa_led_pattern import AlexaLedPattern/' mic_hat_4/pixels.py
sed -i -e 's/from google_home_led_pattern import GoogleHomeLedPattern//' mic_hat_4/pixels.py

touch mic_hat_4/__init__.py

cd ..

cat << EOF > "startAssistantControl.sh"
#!/bin/bash
source env/bin/activate
cd AssistantControl
python AssistantControl.py resources/alexa.umdl resources/snowboy.umdl
EOF

chmod +x startAssistantControl.sh