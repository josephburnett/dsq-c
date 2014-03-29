#!/bin/bash

set -e

mkdir -p ../build/
gcc -o ../build/engine *.c -lzmq -lprotobuf-c

