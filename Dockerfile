# This file describes how to build node-opencv into a runnable linux container with all dependencies installed
# To build:
# 1) Install docker (http://docker.io)
# 2) Build: wget https://raw.github.com/dotcloud/docker/v0.1.6/contrib/docker-build/docker-build && python docker-build $USER/node-opencv < Dockerfile
# 3) Test: docker run $USER/node-opencv node -e "console.log(require('opencv').version)"
#
# VERSION		0.1
# DOCKER-VERSION	0.1.6

from	ubuntu:12.04
run	echo "deb http://archive.ubuntu.com/ubuntu precise main universe" > /etc/apt/sources.list
run	apt-get update
run	apt-get install -q -y libcv-dev
run	apt-get install -q -y libopencv-dev
run	apt-get install -q -y libhighgui-dev
run	apt-get install wget
run	wget -O - http://nodejs.org/dist/v0.8.23/node-v0.8.23-linux-x64.tar.gz | tar -C /usr/local/ --strip-components=1 -zxv
run	npm install opencv
