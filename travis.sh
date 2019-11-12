#!/usr/bin/env bash

set -e

# Submodules
git submodule update --init

# Doxygen xml files
cd example
doxygen
cd ..

if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
  DOXYBOOK_TESTS=ON
  DOXYBOOK_STATIC_STDLIB=ON
else
  DOXYBOOK_TESTS=OFF
  DOXYBOOK_STATIC_STDLIB=OFF
fi

# Build
mkdir build
cd build
cmake -G "Unix Makefiles" \
    -DDOXYBOOK_TESTS=$DOXYBOOK_TESTS \
    -DDOXYBOOK_STATIC_STDLIB=$DOXYBOOK_STATIC_STDLIB \
    -DBUILD_TESTS=OFF \
    -DBUILD_TESTING=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DCMAKE_INSTALL_PREFIX=../install \
    ..
cmake --build . --target install

# Test
if [[ "$TRAVIS_OS_NAME" == "linux" ]]; then
  ctest --verbose
fi

# Package
cd ../install
zip -r ../doxybook2.zip *
