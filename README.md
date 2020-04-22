# [Sean Parent](https://sean-parent.stlab.cc/)

View this blog at <https://sean-parent.stlab.cc/>

## Running Tools
```
docker run --mount type=bind,source="$(pwd)",target=/mnt/docs-src  -t -i -p 3000:3000 -p 3001:3001 -p 8888:8888 docker.pkg.github.com/sean-parent/sean-parent.github.io/tools:latest  bash
```

## Updating docker package
```
docker run --mount type=bind,source="$(pwd)",target=/mnt/docs-src -t -i \
  docker.pkg.github.com/sean-parent/jupyter-docker/docs-tool-cpp-base:latest bash

cd /mnt/docs-src
./tools/update.sh --lock
exit

VERSION="1.1.1"
echo $VERSION > ./VERSION
docker build -t docker.pkg.github.com/sean-parent/sean-parent.github.io/tools:latest .
docker tag docker.pkg.github.com/sean-parent/sean-parent.github.io/tools:latest \
    docker.pkg.github.com/sean-parent/sean-parent.github.io/tools:$VERSION
docker push docker.pkg.github.com/sean-parent/sean-parent.github.io/tools:latest
docker push docker.pkg.github.com/sean-parent/sean-parent.github.io/tools:$VERSION

```
