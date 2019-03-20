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


# Below build, coverage and clean tasks were partly lifted from https://github.com/geo-data/node-mapserv/blob/e99b23a44d910d444f5a45d144859758f820e1d1/Makefile
# @author danschultzer

# The location of the `istanbul` JS code coverage framework. Try and get a
# globally installed version, falling back to a local install.
ISTANBUL := $(shell which istanbul)
ifeq ($(ISTANBUL),)
	ISTANBUL = ./node_modules/.bin/istanbul/lib/cli.js
endif

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

# Perform the code coverage
cover: coverage/index.html
coverage/index.html: coverage/node-opencv.info
	genhtml --output-directory coverage coverage/node-opencv.info
	@echo "\033[0;32mPoint your browser at \`coverage/index.html\`\033[m\017"
coverage/node-opencv.info: coverage/bindings.info
	lcov --test-name node-opencv \
	--add-tracefile coverage/lcov.info \
	--add-tracefile coverage/bindings.info \
	--output-file coverage/node-opencv.info
coverage/bindings.info: coverage/addon.info
	lcov --extract coverage/addon.info '*opencv/src/*' --output-file coverage/bindings.info
coverage/addon.info: coverage/lcov.info
	lcov --capture --base-directory build/ --directory . --output-file coverage/addon.info
# This generates the JS lcov info as well as gcov `*.gcda` files:
coverage/lcov.info: $(test_deps) $(ISTANBUL)
	NODE_OPENCV_DEBUG=true $(NODE) --nouse_idle_notification --expose-gc \
	$(ISTANBUL) cover --report lcovonly -- test/unit.js

$(NODE_PRE_GYP):
	npm install node-pre-gyp

$(ISTANBUL): package.json
	npm install istanbul
	@touch $(ISTANBUL)

# Clean up any generated files
clean: $(NODE_PRE_GYP)
	$(NODE_PRE_GYP) clean
	rm -rf coverage
	rm -rf build
