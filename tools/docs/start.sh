#!/bin/bash

POSITIONAL=()
OPTIONS=""
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -d|--drafts)
        OPTIONS+=" --drafts"
        shift # past argument
    ;;
    *)    # unknown option
        POSITIONAL+=("$1") # save it in an array for later
        shift # past argument
    ;;
esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters

# end command processing
#bundle exec jekyll build

bundle exec jekyll build --watch --incremental $OPTIONS &
sleep 10
browser-sync start --config bs-config.js
