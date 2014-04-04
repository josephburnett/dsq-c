#!/bin/bash

set -e

rm -r build/

(cd proto && ./build-proto.sh)
(cd engine && ./build-engine.sh)
(cd console && ./build-console.sh)

