#!/bin/bash

POSITIONAL=()
OPTIONS=""
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -l|--lock)
        LOCK=YES
        shift # past argument
    ;;
    *)    # unknown option
        POSITIONAL+=("$1") # save it in an array for later
        shift # past argument
    ;;
esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters

rm ./Gemfile.lock

if [[ $LOCK = YES ]]; then
    bundle lock --update
    bundle lock --add-platform x86_64-linux
else
    bundle update
fi

git submodule update --recursive --remote
