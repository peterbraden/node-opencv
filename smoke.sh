#!/bin/bash
node-waf configure && node-waf build && echo '-- Compiled OK -- 

' && node  smoketest.js && echo '-- Smoke Done, running tests --

' && node test/smoke.js 
