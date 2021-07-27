cmake_minimum_required(VERSION 3.10)
project(Doxybook2Cli)

# Project source files
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

# Get the version string based on the git tags
execute_process(
  COMMAND git describe --always
  OUTPUT_VARIABLE VERSION
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "-- Setting Doxybook version to: ${VERSION}")

# Add the project source files
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "doxybook2")
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/Doxybook)
target_compile_definitions(${PROJECT_NAME} PRIVATE VERSION=${VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

if(DOXYBOOK_STATIC_STDLIB)
  target_link_libraries(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
endif()

# Install step
install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION bin
)

# Libraries
target_link_libraries(${PROJECT_NAME} PUBLIC Doxybook2)
