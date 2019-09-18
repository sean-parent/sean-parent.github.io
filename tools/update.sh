#!/bin/bash

brew update
brew upgrade npm
brew upgrade fswatch
brew upgrade rbenv
npm update -g npm
npm update -g browser-sync
gem update bundler
bundle update

git submodule update --recursive --remote
