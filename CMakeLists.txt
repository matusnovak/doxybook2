cmake_minimum_required(VERSION 3.10)
set(CMAKE_MODULE_PATH 
  ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_LIST_DIR}/modules"
)

project(Doxybook2)

if(APPLE OR MSVC)
  set(DOXYBOOK_STATIC_STDLIB OFF)
else()
  option(DOXYBOOK_STATIC_STDLIB "Use static stdlib" ON)
endif()

option(DOXYBOOK_TESTS "Build Doxybook2 tests" OFF)

set(CMAKE_BUILD_TYPE "MinSizeRel" CACHE STRING "Select build type")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/Doxybook)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/DoxybookCli)
if(DOXYBOOK_TESTS)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests/DoxybookTests)
endif()
