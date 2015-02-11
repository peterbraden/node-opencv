VERSION := $(shell node -e "console.log(require('./package.json').version)")

.PHONY: default release smoke

test:
	npm test

smoke:
	npm install --build-from-source
	node smoke/smoketest.js
	npm test


release:
	@echo "Tagging release $(VERSION)"
	@git tag -m "$(VERSION)" v$(VERSION)

	@echo "Pushing tags to GitHub"
	@git push --tags

	@echo "Switching to osx-binaries branch"
	@git checkout osx-binaries

	@echo "Merging master into osx-binaries"
	@git merge --no-ff --commit -m "Merge master into osx-binaries [publish binary]" master

	@echo "Pushing osx-binaries"
	@git push

	@echo "Switching to master branch"
	@git checkout master

	@echo "Publishing to NPM"
	@npm publish
