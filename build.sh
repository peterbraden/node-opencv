#!/bin/bash

node-waf configure build &&
cd examples &&
#node face_detection.js
node $1
