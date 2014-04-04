#!/bin/bash

set -e

mkdir -p ../build/proto/
protoc-c --c_out=../build/proto/ *.proto
protoc --go_out=../build/proto/ *.proto
