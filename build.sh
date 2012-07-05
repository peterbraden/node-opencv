#!/bin/bash

node-gyp rebuild &&
cd examples &&
#node face_detection.js
node $1
