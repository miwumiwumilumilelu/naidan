cmake_minimum_required(VERSION 3.20)

project(
  naidan
  VERSION 0.1.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")
include(AddNaidan)

option(NAIDAN_BUILD_TESTS "Build Naidan tests" ON)

add_subdirectory(include)
add_subdirectory(lib)
add_subdirectory(tools)

if(NAIDAN_BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
  add_subdirectory(unittests)
endif()
