cmake_minimum_required(VERSION 3.0)
project(DoxydownCli)

# Clang tidy
if(USE_CLANG_TIDY)
  set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "clang-tidy;-warnings-as-errors=*;-checks=-*,clang-analyzer-*,-clang-analyzer-alpha*,llvm-*,-llvm-include-order,modernize-*,performance-*,readibility-*,-readability-braces-around-statements,-readability-magic-numbers,-modernize-use-equals-default")
endif()

# Project source files
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

# Add the project source files
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "doxydown")
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/Doxydown)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
foreach(SOURCE ${SOURCES} ${HEADERS})
  source_group("Sources" FILES ${SOURCE})
endforeach()

# Clang tidy
if(USE_CLANG_TIDY)
  set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CLANG_TIDY "clang-tidy;-warnings-as-errors=*;-checks=-*,clang-analyzer-*,-clang-analyzer-alpha*,llvm-*,-llvm-include-order,modernize-*,performance-*,readibility-*,-readability-braces-around-statements,-readability-magic-numbers,-modernize-use-equals-default")
endif()

# Install step
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION bin
)

# Multiprocessor compilation
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
endif()

# Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Doxydown)
target_link_libraries(${PROJECT_NAME} PRIVATE argagg)

