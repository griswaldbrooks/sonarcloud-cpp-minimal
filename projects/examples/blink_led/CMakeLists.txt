cmake_minimum_required(VERSION 3.14)

# Blink LED Example - Platform-agnostic C++ + Arduino Pattern
project(blink_led_example VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# BlinkController library (platform-agnostic)
add_library(blink_controller
    lib/src/blink_controller.cpp
)

target_include_directories(blink_controller PUBLIC
    lib/include
)

# Compiler warnings
target_compile_options(blink_controller PRIVATE
    -Wall -Wextra -Wpedantic -Werror
)

# Coverage flags for library
if(ENABLE_COVERAGE)
    target_compile_options(blink_controller PRIVATE --coverage)
    target_link_options(blink_controller PRIVATE --coverage)
endif()

# Tests (desktop only)
if(BUILD_TESTS)
    enable_testing()

    # Fetch GoogleTest
    include(FetchContent)
    FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)

    # Test executable
    add_executable(test_blink_controller
        test/test_blink_controller.cpp
    )

    target_link_libraries(test_blink_controller
        blink_controller
        GTest::gtest_main
    )

    target_include_directories(test_blink_controller PRIVATE
        test
    )

    # Coverage flags for test executable
    if(ENABLE_COVERAGE)
        target_compile_options(test_blink_controller PRIVATE --coverage)
        target_link_options(test_blink_controller PRIVATE --coverage)
    endif()

    # Register with CTest
    add_test(NAME BlinkControllerTests COMMAND test_blink_controller)
endif()
