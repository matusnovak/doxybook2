cmake_minimum_required(VERSION 3.0)
project(Doxybook2)

# Clang tidy
if(USE_CLANG_TIDY)
  set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "clang-tidy;-warnings-as-errors=*;-checks=-*,clang-analyzer-*,-clang-analyzer-alpha*,llvm-*,-llvm-include-order,modernize-*,performance-*,readibility-*,-readability-braces-around-statements,-readability-magic-numbers,-modernize-use-equals-default")
endif()

# Project source files
set(DOXYDOWN_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${DOXYDOWN_INCLUDE_DIR}/Doxybook/*.hpp)

# Add the project source files
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "doxybook2")
target_include_directories(${PROJECT_NAME} PUBLIC ${DOXYDOWN_INCLUDE_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

# Clang tidy
if(USE_CLANG_TIDY)
  set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "clang-tidy;-warnings-as-errors=*;-checks=-*,clang-analyzer-*,-clang-analyzer-alpha*,llvm-*,-llvm-include-order,modernize-*,performance-*,readibility-*,-readability-braces-around-statements,-readability-magic-numbers,-modernize-use-equals-default")
endif()

# Multiprocessor compilation
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
endif()

# Install
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
  DESTINATION include FILES_MATCHING PATTERN "*.hpp"
)

# Libraries
target_link_libraries(${PROJECT_NAME} PUBLIC fmt tinyxml2 inja json termcolor)
if(MSVC)
  target_link_libraries(${PROJECT_NAME} PRIVATE dirent)
endif()
