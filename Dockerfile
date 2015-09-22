# This file describes how to build node-opencv into a runnable linux container with all dependencies installed
# To build:
# 1) Install docker (http://docker.io)
# 2) Build: wget https://raw.github.com/dotcloud/docker/v0.1.6/contrib/docker-build/docker-build && python docker-build $USER/node-opencv < Dockerfile
# 3) Test: docker run $USER/node-opencv node -e "console.log(require('opencv').version)"
#
# VERSION		0.1
# DOCKER-VERSION	0.1.6

from	ubuntu:12.04
run apt-get update -qq
run apt-get install -y software-properties-common python-software-properties
run add-apt-repository -y ppa:kubuntu-ppa/backports
run apt-get update
run apt-get install -y libcv-dev libcvaux-dev libhighgui-dev libopencv-dev
run curl -sL https://deb.nodesource.com/setup | bash -
run apt-get install -y nodejs
run	npm install opencv || cat npm-debug.log
run add . /root/node-opencv
