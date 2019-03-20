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

# The location of the `node-pre-gyp` module builder. Try and get a globally
# installed version, falling back to a local install.
NODE_PRE_GYP = $(shell which node-pre-gyp)
ifeq ($(NODE_GYP),)
	NODE_PRE_GYP = ./node_modules/.bin/node-pre-gyp
endif

NODE := $(shell which node)
test_deps = build \
./test/*.js \
./lib/*.js \
$(NODE)

build: build/Debug/opencv.node
build/Debug/opencv.node:
	$(NODE_PRE_GYP) --verbose --debug rebuild

$(NODE_PRE_GYP):
	npm install node-pre-gyp

# Clean up any generated files
clean: $(NODE_PRE_GYP)
	$(NODE_PRE_GYP) clean
	rm -rf coverage
	rm -rf build
