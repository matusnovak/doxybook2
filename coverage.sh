#!/bin/bash

set -e

lcov --directory ./build --capture --output-file ./build/coverage.info
lcov --remove ./build/coverage.info '/usr/*' "${HOME}"'/.cache/*' '*/tinyxml2/*' '*/nlohmann/*' '*/fmt/*' '*/catch2/*' '*/rang.hpp' '*/Log.cpp' --output-file ./build/coverage.info
lcov --list ./build/coverage.info
