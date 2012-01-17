#!/bin/bash
node-waf configure && node-waf build && echo '-- Compiled OK -- 

'  && node test/smoke.js && echo '-- Tests Done, running smoke --

' && node smoketest.js
