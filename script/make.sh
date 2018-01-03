#!/bin/bash

set -u
set -e
set -o pipefail

dirname=$(dirname "${BASH_SOURCE[0]}")
dirname=$(cd "$dirname" && pwd)

build_dir="${dirname}/../build"
cd "$build_dir"
cmake ..
make

