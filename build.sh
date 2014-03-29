#!/bin/bash

set -e

BUILD_DIR=$PWD

(cd proto && ./build-proto.sh)
(cd engine && ./build-engine.sh)

