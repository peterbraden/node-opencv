# This file describes how to build node-opencv into a runnable linux container with all dependencies installed
# To build:
# 1) Install docker (http://docker.io)
# 2) Build: wget https://raw.github.com/dotcloud/docker/v0.1.6/contrib/docker-build/docker-build && python docker-build $USER/node-opencv < Dockerfile
# 3) Test: docker run $USER/node-opencv node -e "console.log(require('opencv').version)"
#
# VERSION		0.3
# DOCKER-VERSION	8.1.2


# update to 14.04
FROM	ubuntu:14.04

# listing myself as maintainer of _this_ Dockerfile, though I am not the author of the install script (credit to http://rodrigoberriel.com/)
MAINTAINER borromeotlhs@gmail.com

# run Rodrigo Berriel’s script for installing opencv3 on Ubuntu 14.04
# I’ll convert this into a full Dockerfile later, once I see if it works
#COPY ./install-opencv3.sh /tmp/

# this is needed as libfaac-dev comes from multiverse, according to:
# http://superuser.com/questions/467774/how-to-install-libfaac-dev
RUN echo "deb http://us.archive.ubuntu.com/ubuntu/ precise multiverse\n\
deb-src http://us.archive.ubuntu.com/ubuntu/ precise multiverse\n\
deb http://us.archive.ubuntu.com/ubuntu/ precise-updates multiverse\n\
deb-src http://us.archive.ubuntu.com/ubuntu/ precise-updates multiverse\n"\
>> /etc/apt/sources.list

RUN apt-get update -qq
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y \
    software-properties-common
RUN add-apt-repository ppa:george-edison55/cmake-3.x
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y \
curl \
cmake \
wget \
unzip \
libopencv-dev \
build-essential \
git \
libgtk2.0-dev \
pkg-config \
python-dev \
python-numpy \
libdc1394-22 \
libdc1394-22-dev \
libjpeg-dev \
libpng12-dev \
libtiff4-dev \
libjasper-dev \
libavcodec-dev \
libavformat-dev \
libswscale-dev \
libxine-dev \
libgstreamer0.10-dev \
libgstreamer-plugins-base0.10-dev \
libv4l-dev \
libtbb-dev \
libqt4-dev \
libfaac-dev \
libmp3lame-dev \
libopencore-amrnb-dev \
libopencore-amrwb-dev \
libtheora-dev \
libvorbis-dev \
libxvidcore-dev \
x264 \
v4l-utils

RUN mkdir opencv
WORKDIR opencv

RUN wget https://github.com/Itseez/opencv/archive/3.0.0.zip -O opencv-3.0.0.zip
RUN unzip opencv-3.0.0.zip
RUN mkdir opencv-3.0.0/build
WORKDIR opencv-3.0.0/build

RUN cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D BUILD_NEW_PYTHON_SUPPORT=ON -D WITH_V4L=ON -D WITH_OPENGL=ON ..

RUN make -j $(nproc) && make install
RUN echo "/usr/local/lib" > /etc/ld.so.conf.d/opencv.conf
RUN ldconfig
