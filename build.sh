#!/usr/bin/env bash

set -e

git submodule update --init
mkdir build
cd build
cmake -G "Unix Makefiles" -DDOXYDOWN_TESTS=ON -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_INSTALL_PATH=../install ..
cmake --build .

