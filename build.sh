#!/bin/sh

set -e
set -x

docker run --rm -it -v "${PWD}:/src" msalau/rl78-elf-gcc:latest make "$@"
