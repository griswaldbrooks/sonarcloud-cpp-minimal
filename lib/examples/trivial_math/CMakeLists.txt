cmake_minimum_required(VERSION 3.14)
project(TrivialMath VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library with implementation
add_library(trivial_math src/trivial_math.cpp)
target_include_directories(trivial_math PUBLIC include)

# Coverage flags for library
if(ENABLE_COVERAGE)
    target_compile_options(trivial_math PRIVATE --coverage)
    target_link_options(trivial_math PRIVATE --coverage)
endif()

# Tests
if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(test_trivial_math test/test_trivial_math.cpp)
    target_link_libraries(test_trivial_math trivial_math GTest::gtest_main)

    # Coverage flags
    if(ENABLE_COVERAGE)
        target_compile_options(test_trivial_math PRIVATE --coverage)
        target_link_options(test_trivial_math PRIVATE --coverage)
    endif()

    add_test(NAME TrivialMathTests COMMAND test_trivial_math)
endif()
