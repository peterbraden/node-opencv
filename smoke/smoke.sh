#!/bin/bash

if [ ! -f smoke/smoketest.js ]; then
	echo "Please run smoke test from the top-level folder of the repository." >&2
	exit 1
fi

node-gyp build && echo '-- Compiled OK -- 

' && node  smoke/smoketest.js && echo '-- Smoke Done, running tests --

' && npm test # && echo '-- Tests Run, runnning examples --
#(building example data)
#' && ./examples/make-example-files.sh && node examples/motion-track.js 
