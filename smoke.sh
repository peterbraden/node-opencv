#!/bin/bash
node-gyp -d rebuild && echo '-- Compiled OK -- 

' && node  smoketest.js && echo '-- Smoke Done, running tests --

' && npm test
