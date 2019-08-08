#!/bin/bash

npm update -g npm
npm update -g browser-sync
gem update bundler

git submodule update --recursive --remote

bundle update
