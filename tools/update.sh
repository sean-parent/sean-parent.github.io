#!/bin/bash

rbenv local $(rbenv install -l | grep -v - | tail -1)
gem install bundler
rbenv rehash
bundle update

# what to do about submodules?

#brew update
# brew upgrade npm
# brew upgrade fswatch
# brew upgrade rbenv
# npm update -g npm
# npm update -g browser-sync
# gem update bundler
# bundle update
# 
# git submodule update --recursive --remote
