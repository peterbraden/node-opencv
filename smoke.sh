#!/bin/bash
node-waf configure && node-waf build && echo '-- Compiled OK -- 

'  && node smoketest