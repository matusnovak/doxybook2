cmake_minimum_required(VERSION 3.0)
project(DoxydownTests)

# Clang tidy
if(USE_CLANG_TIDY)
  set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "clang-tidy;-warnings-as-errors=*;-checks=-*,clang-analyzer-*,-clang-analyzer-alpha*,llvm-*,-llvm-include-order,modernize-*,performance-*,readibility-*,-readability-braces-around-statements,-readability-magic-numbers,-modernize-use-equals-default")
endif()

# Project source files
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

# Add the project source files
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/Doxydown)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
foreach(SOURCE ${SOURCES} ${HEADERS})
  source_group("Sources" FILES ${SOURCE})
endforeach()

# Clang tidy
if(USE_CLANG_TIDY)
  set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "clang-tidy;-warnings-as-errors=*;-checks=-*,clang-analyzer-*,-clang-analyzer-alpha*,llvm-*,-llvm-include-order,modernize-*,performance-*,readibility-*,-readability-braces-around-statements,-readability-magic-numbers,-modernize-use-equals-default")
endif()

# Multiprocessor compilation
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
endif()

# Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Doxydown)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
target_compile_definitions(${PROJECT_NAME} PRIVATE IMPORT_DIR="${CMAKE_SOURCE_DIR}/example/doxygen/xml")
