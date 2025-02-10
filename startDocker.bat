set VOLUME=sean-parent.github.io
docker run --mount type=bind,source="%CD%",target=/mnt/host --tty --interactive --publish 3000-3001:3000-3001 %VOLUME% bash