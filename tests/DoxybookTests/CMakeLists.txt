cmake_minimum_required(VERSION 3.10)
project(Doxybook2Tests)

# Dependencies
find_package(Catch2 CONFIG REQUIRED)
find_package(nlohmann_json_schema_validator REQUIRED)

# Project source files
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

# Add the project source files
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/Doxybook)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# Copy xml files used by tests
add_custom_target(${PROJECT_NAME}CopyFiles ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/xml ${CMAKE_CURRENT_BINARY_DIR}/xml
    DEPENDS ${PROJECT_NAME})

if(DOXYBOOK_STATIC_STDLIB)
  target_link_libraries(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
endif()

# Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Doxybook2 Catch2::Catch2 nlohmann_json_schema_validator)

# Testing
enable_testing()
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
target_compile_definitions(${PROJECT_NAME} PRIVATE IMPORT_DIR="${CMAKE_SOURCE_DIR}/example/doxygen/xml")
