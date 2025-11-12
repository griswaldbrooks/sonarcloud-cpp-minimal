cmake_minimum_required(VERSION 3.14)
project(sonarcloud_cpp_minimal CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable coverage and export compile commands
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add library
add_library(math src/math.cpp)
target_include_directories(math PUBLIC src)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Add tests
enable_testing()

add_executable(test_math test/test_math.cpp)
target_link_libraries(test_math math gtest_main gcov)

include(GoogleTest)
gtest_discover_tests(test_math)
