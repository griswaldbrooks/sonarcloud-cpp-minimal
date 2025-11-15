cmake_minimum_required(VERSION 3.14)

# Blink LED Example - Platform-agnostic C++ + Arduino Pattern
project(blink_led_example VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# BlinkController library (header-only template, platform-agnostic)
add_library(blink_controller INTERFACE)

target_include_directories(blink_controller INTERFACE
    lib/include
)

# ConsoleSimulator library (header-only, testable console utilities)
add_library(console_simulator INTERFACE)

target_include_directories(console_simulator INTERFACE
    lib/include
)

# Note: INTERFACE libraries don't support compile options or coverage flags
# Coverage is applied at the test and demo executable levels below

# Demo executable (desktop only)
# Demonstrates BlinkController with ConsoleLEDPin for visual feedback
add_executable(blink_demo
    src/main.cpp
)

target_link_libraries(blink_demo
    blink_controller
    console_simulator
)

# Coverage flags for demo executable
if(ENABLE_COVERAGE)
    target_compile_options(blink_demo PRIVATE --coverage)
    target_link_options(blink_demo PRIVATE --coverage)
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

    # Test executable - blink_controller
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

    # Test executable - console_simulator
    add_executable(test_console_simulator
        test/test_console_simulator.cpp
    )

    target_link_libraries(test_console_simulator
        console_simulator
        GTest::gtest_main
    )

    # Coverage flags for test executable
    if(ENABLE_COVERAGE)
        target_compile_options(test_console_simulator PRIVATE --coverage)
        target_link_options(test_console_simulator PRIVATE --coverage)
    endif()

    # Register with CTest
    add_test(NAME ConsoleSimulatorTests COMMAND test_console_simulator)
endif()
