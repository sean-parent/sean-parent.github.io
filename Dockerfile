FROM docker.pkg.github.com/sean-parent/jupyter-docker/docs-tool-cpp-base:1.0.0
USER app

RUN mkdir ./install
WORKDIR ./install
COPY ./Gemfile .
COPY ./Gemfile.lock .
COPY .ruby-version .

RUN (eval "$(rbenv init -)"; bundle install)
WORKDIR ~
