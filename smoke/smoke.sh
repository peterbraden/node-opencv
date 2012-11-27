#!/bin/bash
node-gyp build && echo '-- Compiled OK -- 

' && node examples/motion-track.js && 0 && node  smoke/smoketest.js && echo '-- Smoke Done, running tests --

' && npm test
