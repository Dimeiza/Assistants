CURRENT_DIR="$(pwd)"
INSTALL_BASE=${INSTALL_BASE:-"$CURRENT_DIR"}

mkdir avs-device-sdk

cd avs-device-sdk
mkdir sdk-build sdk-source third-party sdk-install db 

sudo apt-get update

sudo apt-get -y install \
git gcc cmake build-essential libsqlite3-dev libcurl4-openssl-dev libfaad-dev \
libssl-dev libsoup2.4-dev libgcrypt20-dev libgstreamer-plugins-bad1.0-dev \
gstreamer1.0-plugins-good libasound2-dev doxygen

cd $INSTALL_BASE/avs-device-sdk/third-party

wget -c http://www.portaudio.com/archives/pa_stable_v190600_20161030.tgz
tar zxf pa_stable_v190600_20161030.tgz

cd portaudio
./configure --without-jack

make

pip install commentjson

cd $INSTALL_BASE/avs-device-sdk/sdk-source
git clone git://github.com/alexa/avs-device-sdk.git --branch v1.19.0

# patch the SampleApp 
cp $INSTALL_BASE/patch/Alexa/PosixQueueManager.cpp $INSTALL_BASE/avs-device-sdk/sdk-source/avs-device-sdk/SampleApp/src/
cp $INSTALL_BASE/patch/Alexa/PosixQueueManager.h $INSTALL_BASE/avs-device-sdk/sdk-source/avs-device-sdk/SampleApp/include/SampleApp/

cp $INSTALL_BASE/patch/Alexa/diff.patch $INSTALL_BASE/avs-device-sdk/sdk-source/avs-device-sdk/
cd $INSTALL_BASE/avs-device-sdk/sdk-source/avs-device-sdk/
patch -p1 < diff.patch
cd ..

cd $INSTALL_BASE/avs-device-sdk/sdk-build
cmake $INSTALL_BASE/avs-device-sdk/sdk-source/avs-device-sdk \
-DGSTREAMER_MEDIA_PLAYER=ON \
-DPORTAUDIO=ON \
-DPORTAUDIO_LIB_PATH=$INSTALL_BASE/avs-device-sdk/third-party/portaudio/lib/.libs/libportaudio.a \
-DPORTAUDIO_INCLUDE_DIR=$INSTALL_BASE/avs-device-sdk/third-party/portaudio/include

make SampleApp -j1

cd $INSTALL_BASE
cp config.json $INSTALL_BASE/avs-device-sdk/sdk-source/avs-device-sdk/tools/Install
cd $INSTALL_BASE/avs-device-sdk/sdk-source/avs-device-sdk/tools/Install 
    
SOUND_CONFIG="$HOME/.asoundrc"
START_SCRIPT="$INSTALL_BASE/startsample.sh"

echo
echo "==============> SAVING AUDIO CONFIGURATION FILE =============="
echo

bash genConfig.sh config.json 12345 \
 $INSTALL_BASE/avs-device-sdk/db \
 $INSTALL_BASE/avs-device-sdk/sdk-source/avs-device-sdk \
 $INSTALL_BASE/avs-device-sdk/sdk-build/Integration/AlexaClientSDKConfig.json \
 -DSDK_CONFIG_MANUFACTURER_NAME="raspberrypi" \
 -DSDK_CONFIG_DEVICE_DESCRIPTION="raspberrypi"

sed -i "2s/^/\n    \"gstreamerMediaPlayer\":{\n        \"audioSink\":\"alsasink\"\n    },\n/" $INSTALL_BASE/avs-device-sdk/sdk-build/Integration/AlexaClientSDKConfig.json 

cat << EOF > "$SOUND_CONFIG"
pcm.!default {
  type asym
   playback.pcm {
     type plug
     slave.pcm "hw:0,0"
   }
   capture.pcm {
     type plug
     slave.pcm "hw:1,0"
   }
}
EOF

cat << EOF > "$START_SCRIPT"
#!/bin/bash
cd "$INSTALL_BASE/avs-device-sdk/sdk-build/"

./SampleApp/src/SampleApp ./Integration/AlexaClientSDKConfig.json DEBUG9

EOF
chmod +x "$START_SCRIPT"

