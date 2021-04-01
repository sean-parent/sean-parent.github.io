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
bundle exec jekyll build --profile $OPTIONS
