# [Sean Parent](https://sean-parent.stlab.cc/)

View this blog at <https://sean-parent.stlab.cc/>

## Running Tools
```
docker run --mount type=bind,source="$(pwd)",target=/mnt/docs-src  -t -i -p 3000:3000 -p 3001:3001 -p 8888:8888 docker.pkg.github.com/sean-parent/sean-parent.github.io/tools:1.0.0  bash
```

## Updating docker package
```
docker build -t docker.pkg.github.com/sean-parent/sean-parent.github.io/tools:1.0.0 .
```
