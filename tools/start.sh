#!/bin/bash

{
	bundle exec jekyll build
	bundle exec jekyll build --watch --incremental
} & {
    browser-sync start --config bs-config.js
} & wait
