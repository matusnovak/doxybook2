#!/usr/bin/env bash

set -e

# Submodules
git submodule update --init

# Doxygen xml files
cd example
doxygen
cd ..

# Build
mkdir build
cd build
cmake -G "Unix Makefiles" \
    -DDOXYDOWN_TESTS=ON \
    -DBUILD_TESTS=OFF \
    -DBUILD_TESTING=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DCMAKE_INSTALL_PREFIX=../install \
    ..
cmake --build . --target install

# Test
ctest --verbose

# Package
cd ../install
zip -r ../doxybook2.zip *
