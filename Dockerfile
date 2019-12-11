FROM docker.pkg.github.com/sean-parent/jupyter-docker/docs-tool-cpp-base:latest
USER app
WORKDIR /home/app

RUN mkdir ./install
WORKDIR ./install
COPY ./Gemfile .
COPY ./Gemfile.lock .
COPY ./.ruby-version .

RUN  (eval "$(rbenv init -)"; \
    gem install bundler; \
    rbenv rehash; \
    bundle install)

EXPOSE 3000 3001

ADD VERSION .
