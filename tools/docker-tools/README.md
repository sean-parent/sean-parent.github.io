# Using the docker image

## Setup

### Install Docker
If you don't already have docker installed, [install Docker](https://docs.docker.com/get-docker/).

### Building the docker image

To build the docker image, first update the VERSION variable below (please use semantic versioning). Add a [release note](#release-notes).

```
VERSION="1.0.9"
echo $VERSION > ./tools/docker-tools/VERSION

VOLUME="sean-parent.github.io"

# Specify the ruby version to match https://pages.github.com/versions/
# The docker file is currently building from ubuntu:impish instead of latest because
# past that, Ruby 2.7.3 is not supported. If the ruby requirement is updated, then move to
# latest.

RUBY_VERSION="2.7.3"
echo $RUBY_VERSION > ./.ruby-version

# build the base image, no-cache is used so the latest tools are installed
docker build --build-arg RUBY_VERSION=$RUBY_VERSION --file ./tools/docker-tools/Dockerfile \
  --target base --tag $VOLUME . \
  --no-cache 

# update the docs environment
docker run --mount type=bind,source="$(pwd)",target=/mnt/host --tty --interactive $VOLUME bash

cd /mnt/host
./tools/docs/update.sh --lock
exit

# build the final image
docker build --build-arg RUBY_VERSION=$RUBY_VERSION \
  --file ./tools/docker-tools/Dockerfile --target full --tag $VOLUME .
```

## Running the Docker image

To run the docker image, execute the following.

```
VOLUME="sean-parent.github.io"
docker run --mount type=bind,source="$(pwd)",target=/mnt/host \
    --tty --interactive --publish 3000-3001:3000-3001 \
    $VOLUME bash
```

This should leave you at bash prompt that looks like:

```
app@fc7590a63ba3:~$
```

The hex number is the docker image container ID and may be different. Going forward I refer to this as the _docker_ prompt to distinguish it from the _local_ prompt.

## Build the documentation site

To build or rebuild the complete documentation site locally execute the following from the docker prompt:

```
cd /mnt/host
./tools/docs/prepare.sh
```

## Run a local server for the site

Once the site has been prepared, you can run it to see how it looks. From the docker prompt enter:

```
cd /mnt/host
./tools/docs/start.sh --drafts
```

To view the site, open a browser to `http://localhost:3000`. The site will auto rebuild and refresh as files are changed. The [Atom editor](https://atom.io/) has a nice [language package for markdown](https://atom.io/packages/language-markdown) that understand the YAML front matter that Jekyll uses, as well as a core package for markdown previews that uses the github style (great for editing readme files).

## Tips

If you want to open another terminal on the running image use:

```
docker ps
docker exec -it <container id> bash
```

To test a local copy of the jekyll theme, edit the Gemfile and use:

```
docker run --mount type=bind,source="$(pwd)",target=/mnt/host \
    --mount type=bind,source=$HOME/Projects/github.com/adobe/hyde-theme,target=/mnt/themes \
    --tty --interactive --publish 3000-3001:3000-3001 \
    $VOLUME bash
```

### Release Notes

- 1.0.0 - Initial release for jekyll
- 1.0.1 - First update
- 1.0.3 - Updating jekyll tooling
- 1.0.3 - Updating jekyll tooling
- 1.0.5 - Updating jekyll tooling
- 1.0.8 - Updating tooling, locking to ubuntu:impish, draft of exceptions talk, adding recent talks to page
- 1.0.9 - Updating tooling
