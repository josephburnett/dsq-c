#!/bin/bash

set -e

mkdir -p ../build/proto/
protoc-c --c_out=../build/proto/ *.proto
