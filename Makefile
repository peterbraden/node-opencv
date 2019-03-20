VERSION := $(shell node -e "console.log(require('./package.json').version)")

test:
	npm test
.PHONY: test

smoke:
	npm install --build-from-source
	node smoke/smoketest.js
	npm test
.PHONY: smoke

travis-build:
	docker build -t peterbraden/node-opencv-ubuntu-12-04 -f test/Dockerfile-ubuntu-12-04 .
	docker build -t peterbraden/node-opencv-ubuntu-14-04 -f test/Dockerfile-ubuntu-14-04 .
.PHONY: travis-build
