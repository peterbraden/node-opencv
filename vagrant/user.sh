export REPO_FOLDER="node-opencv"
cd /home/vagrant/$REPO_FOLDER
apt-get install -y cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
mkdir opencv_lib
cd /home/vagrant/$REPO_FOLDER/opencv_lib/
[ ! -f /home/vagrant/$REPO_FOLDER/opencv_lib/2.4.11.tar.gz ] && wget https://github.com/Itseez/opencv/archive/2.4.11.tar.gz && tar -zxvf2.4.11.tar.gz
cd opencv-2.4.11
mkdir release
cd release
echo "Removing any pre-installed ffmpeg and x264"
apt-get -y remove ffmpeg x264 libx264-dev
echo "Installing Dependenices"
apt-get -y install libopencv-dev
apt-get -y install build-essential checkinstall cmake pkg-config yasm
apt-get -y install libtiff4-dev libjpeg-dev libjasper-dev
apt-get -y install libavcodec-dev libavformat-dev libswscale-dev libdc1394-22-dev libdc1394-22 libdc1394-utils libxine-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libv4l-dev
apt-get -y install python-dev python-numpy
apt-get -y install libtbb-dev
apt-get -y install libqt4-dev libgtk2.0-dev
apt-get -y install libfaac-dev libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libtheora-dev libvorbis-dev libxvidcore-dev
apt-get -y install x264 v4l-utils ffmpeg libx264-dev
apt-get -y install libgtk2.0-dev

cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D BUILD_PYTHON_SUPPORT=ON ..
make install
cd /home/vagrant/$REPO_FOLDER/
