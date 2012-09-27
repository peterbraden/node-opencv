#!/bin/bash
node-gyp rebuild && echo '-- Compiled OK -- 

' && node  smoke/smoketest.js && echo '-- Smoke Done, running tests --

' && npm test
