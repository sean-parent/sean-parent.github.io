#!/bin/bash

#bundle exec jekyll build

bundle exec jekyll build --watch --incremental &
sleep 30
browser-sync start --config bs-config.js
