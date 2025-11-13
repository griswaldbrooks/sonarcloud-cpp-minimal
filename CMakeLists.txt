cmake_minimum_required(VERSION 3.14)
project(Halloween2 VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTS "Build tests" ON)
option(ENABLE_COVERAGE "Enable coverage" OFF)

# Enable testing at root level
if(BUILD_TESTS)
    enable_testing()
endif()

# Phase 0 examples
add_subdirectory(lib/examples/trivial_math)
