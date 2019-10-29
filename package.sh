#!/usr/bin/env bash

set -e

cd build
cmake --build . --target INSTALL
cd ..
cd install
zip ../doxydown-$(gcc -dumpmachine).zip *
