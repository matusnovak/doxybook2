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
option(DOXYBOOK_COVERAGE "Enable coverage reporting" OFF)

set(CMAKE_BUILD_TYPE "MinSizeRel" CACHE STRING "Select build type")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")

if(DOXYBOOK_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  # Code Coverage Configuration
  add_library(${PROJECT_NAME}Coverage INTERFACE)

  # Add required flags (GCC & LLVM/Clang)
  target_compile_options(${PROJECT_NAME}Coverage INTERFACE
    -O0        # no optimization
    -g         # generate debug info
    --coverage # sets all required flags
  )
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    target_link_options(${PROJECT_NAME}Coverage INTERFACE --coverage)
  else()
    target_link_libraries(${PROJECT_NAME}Coverage INTERFACE --coverage)
  endif()

  set(DOXYBOOK_COVERAGE_ENABLED ON)
else()
  set(DOXYBOOK_COVERAGE_ENABLED OFF)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/Doxybook)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/DoxybookCli)
if(DOXYBOOK_TESTS)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests/DoxybookTests)
endif()
