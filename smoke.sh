#!/bin/bash
node-waf configure && node-waf -v build && echo '-- Compiled OK -- 

' && node  smoketest.js && echo '-- Smoke Done, running tests --

' && npm test
