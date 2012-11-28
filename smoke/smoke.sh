#!/bin/bash
node-gyp build && echo '-- Compiled OK -- 

' && node  smoke/smoketest.js && echo '-- Smoke Done, running tests --

' && npm test && echo '-- Tests Run, runnning examples --
(building example data)
' && ./examples/make-example-files.sh && node examples/motion-track.js 
