#!/bin/bash
node-gyp rebuild && echo '-- Compiled OK -- 

' && node  smoketest.js && echo '-- Smoke Done, running tests --

' && npm test
