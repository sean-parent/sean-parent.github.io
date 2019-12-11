# [Sean Parent](https://sean-parent.stlab.cc/)

View this blog at <https://sean-parent.stlab.cc/>

## Running Tools
```
docker run --mount type=bind,source="$(pwd)",target=/mnt/docs-src  -t -i -p 3000:3000 -p 3001:3001 -p 8888:8888 docker.pkg.github.com/sean-parent/sean-parent.github.io/tools:1.1.0  bash
```

## Updating docker package
```
docker run --mount type=bind,source="$(pwd)",target=/mnt/docs-src -t -i \
  docker.pkg.github.com/sean-parent/jupyter-docker/docs-tool-cpp-base:latest bash
  
cd /mnt/docs-src
./tools/update.sh
exit

docker build -t docker.pkg.github.com/sean-parent/sean-parent.github.io/tools:latest .
docker push docker.pkg.github.com/sean-parent/sean-parent.github.io/tools:latest



```
