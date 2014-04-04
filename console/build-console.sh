#!/bin/bash

set -e

# go install code.google.com/p/goprotobuf/{proto,protoc-gen-go}

mkdir -p ../build

export GOPATH=`pwd`
go install github.com/josephburnett/dsq/console

