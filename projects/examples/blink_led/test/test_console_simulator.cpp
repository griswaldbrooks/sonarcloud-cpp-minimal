#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include "console_simulator.h"

// Test fixture for console_led_pin tests
struct console_led_pin_test : public ::testing::Test {
protected:
    console_led_pin pin;
};

// Test initial state
TEST_F(console_led_pin_test, initial_state_is_off) {
    EXPECT_FALSE(pin.get_state());
}

// Test state transitions
TEST_F(console_led_pin_test, state_transitions) {
    // Initially off
    EXPECT_FALSE(pin.get_state());

    // Turn on
    pin.set(true);
    EXPECT_TRUE(pin.get_state());

    // Turn off
    pin.set(false);
    EXPECT_FALSE(pin.get_state());

    // Multiple transitions
    pin.set(true);
    EXPECT_TRUE(pin.get_state());
    pin.set(true);  // Stay on
    EXPECT_TRUE(pin.get_state());
    pin.set(false);
    EXPECT_FALSE(pin.get_state());
}

// Test output generation
TEST_F(console_led_pin_test, generates_output_on_set) {
    pin.set(true);
    std::string output = pin.get_last_output();
    EXPECT_FALSE(output.empty());
}

// Test output contains timestamp
TEST_F(console_led_pin_test, output_contains_timestamp) {
    pin.set(true);
    std::string output = pin.get_last_output();
    EXPECT_NE(output.find("ms]"), std::string::npos);
}

// Test output contains LED label
TEST_F(console_led_pin_test, output_contains_led_label) {
    pin.set(true);
    std::string output = pin.get_last_output();
    EXPECT_NE(output.find("LED:"), std::string::npos);
}

// Test output contains ON state
TEST_F(console_led_pin_test, output_shows_on_state) {
    pin.set(true);
    std::string output = pin.get_last_output();
    std::string stripped = console_led_pin::strip_ansi_codes(output);
    EXPECT_NE(stripped.find("ON"), std::string::npos);
}

// Test output contains OFF state
TEST_F(console_led_pin_test, output_shows_off_state) {
    pin.set(false);
    std::string output = pin.get_last_output();
    std::string stripped = console_led_pin::strip_ansi_codes(output);
    EXPECT_NE(stripped.find("OFF"), std::string::npos);
}

// Test ANSI color codes in output
TEST_F(console_led_pin_test, output_contains_ansi_codes) {
    pin.set(true);
    std::string output = pin.get_last_output();
    // ANSI codes start with ESC[
    EXPECT_NE(output.find("\033["), std::string::npos);
}

// Test green color for ON state
TEST_F(console_led_pin_test, on_state_uses_green_color) {
    pin.set(true);
    std::string output = pin.get_last_output();
    // Green color code is \033[32m
    EXPECT_NE(output.find("\033[32m"), std::string::npos);
}

// Test red color for OFF state
TEST_F(console_led_pin_test, off_state_uses_red_color) {
    pin.set(false);
    std::string output = pin.get_last_output();
    // Red color code is \033[31m
    EXPECT_NE(output.find("\033[31m"), std::string::npos);
}

// Test reset color code in output
TEST_F(console_led_pin_test, output_contains_reset_code) {
    pin.set(true);
    std::string output = pin.get_last_output();
    // Reset code is \033[0m
    EXPECT_NE(output.find("\033[0m"), std::string::npos);
}

// Test timestamp reset
TEST_F(console_led_pin_test, reset_time_affects_timestamp) {
    // Get initial timestamp
    uint32_t t1 = pin.get_current_timestamp_ms();

    // Wait a bit
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Timestamp should have increased
    uint32_t t2 = pin.get_current_timestamp_ms();
    EXPECT_GT(t2, t1);

    // Reset
    pin.reset_time();

    // Timestamp should be small again
    uint32_t t3 = pin.get_current_timestamp_ms();
    EXPECT_LT(t3, t2);
    EXPECT_LT(t3, 10);  // Should be very close to 0
}

// Test current timestamp increases
TEST_F(console_led_pin_test, timestamp_increases_monotonically) {
    uint32_t t1 = pin.get_current_timestamp_ms();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    uint32_t t2 = pin.get_current_timestamp_ms();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    uint32_t t3 = pin.get_current_timestamp_ms();

    EXPECT_LE(t1, t2);
    EXPECT_LE(t2, t3);
}

// Test format_output static function
TEST_F(console_led_pin_test, format_output_on_state) {
    std::string output = console_led_pin::format_output(1234, true);
    EXPECT_NE(output.find("1234ms"), std::string::npos);
    std::string stripped = console_led_pin::strip_ansi_codes(output);
    EXPECT_NE(stripped.find("ON"), std::string::npos);
}

TEST_F(console_led_pin_test, format_output_off_state) {
    std::string output = console_led_pin::format_output(5678, false);
    EXPECT_NE(output.find("5678ms"), std::string::npos);
    std::string stripped = console_led_pin::strip_ansi_codes(output);
    EXPECT_NE(stripped.find("OFF"), std::string::npos);
}

TEST_F(console_led_pin_test, format_output_zero_timestamp) {
    std::string output = console_led_pin::format_output(0, true);
    EXPECT_NE(output.find("0ms"), std::string::npos);
}

// Test strip_ansi_codes static function
TEST_F(console_led_pin_test, strip_ansi_codes_removes_color) {
    std::string input = "\033[32mGREEN TEXT\033[0m";
    std::string output = console_led_pin::strip_ansi_codes(input);
    EXPECT_EQ(output, "GREEN TEXT");
}

TEST_F(console_led_pin_test, strip_ansi_codes_handles_plain_text) {
    std::string input = "Plain text without codes";
    std::string output = console_led_pin::strip_ansi_codes(input);
    EXPECT_EQ(output, input);
}

TEST_F(console_led_pin_test, strip_ansi_codes_handles_multiple_codes) {
    std::string input = "\033[31mRED\033[0m and \033[32mGREEN\033[0m";
    std::string output = console_led_pin::strip_ansi_codes(input);
    EXPECT_EQ(output, "RED and GREEN");
}

TEST_F(console_led_pin_test, strip_ansi_codes_handles_empty_string) {
    std::string input = "";
    std::string output = console_led_pin::strip_ansi_codes(input);
    EXPECT_EQ(output, "");
}

// Test fixture for real_time_timer tests
struct real_time_timer_test : public ::testing::Test {
protected:
    real_time_timer timer;
};

// Test initial state
TEST_F(real_time_timer_test, initial_time_is_small) {
    // Timer should start near 0ms
    uint32_t t = timer.millis();
    EXPECT_LT(t, 10);  // Should be less than 10ms
}

// Test time increases
TEST_F(real_time_timer_test, time_increases) {
    uint32_t t1 = timer.millis();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    uint32_t t2 = timer.millis();

    EXPECT_GT(t2, t1);
    EXPECT_GE(t2 - t1, 8);  // At least 8ms should have passed (some tolerance)
}

// Test monotonic increase
TEST_F(real_time_timer_test, monotonic_increase) {
    uint32_t t1 = timer.millis();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    uint32_t t2 = timer.millis();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    uint32_t t3 = timer.millis();

    EXPECT_LE(t1, t2);
    EXPECT_LE(t2, t3);
}

// Test reset functionality
TEST_F(real_time_timer_test, reset_returns_to_zero) {
    // Let some time pass
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    uint32_t t1 = timer.millis();
    EXPECT_GT(t1, 15);  // Should be at least 15ms

    // Reset
    timer.reset();

    // Should be near 0 again
    uint32_t t2 = timer.millis();
    EXPECT_LT(t2, 10);
}

// Test multiple resets
TEST_F(real_time_timer_test, multiple_resets) {
    timer.reset();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_GT(timer.millis(), 5);

    timer.reset();
    EXPECT_LT(timer.millis(), 5);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_GT(timer.millis(), 5);

    timer.reset();
    EXPECT_LT(timer.millis(), 5);
}

// Test reasonable time bounds
TEST_F(real_time_timer_test, reasonable_time_values) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    uint32_t t = timer.millis();

    // Should be at least 40ms (some tolerance)
    EXPECT_GE(t, 40);

    // Should not be wildly high (no more than 200ms)
    EXPECT_LE(t, 200);
}

// Test get_start_time accessor
TEST_F(real_time_timer_test, get_start_time_returns_valid_timepoint) {
    auto start = timer.get_start_time();

    // Start time should be in the past (or very recent)
    auto now = std::chrono::steady_clock::now();
    EXPECT_LE(start, now);
}

// Test start time changes after reset
TEST_F(real_time_timer_test, start_time_updates_after_reset) {
    auto start1 = timer.get_start_time();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    timer.reset();

    auto start2 = timer.get_start_time();

    // Start time should have advanced
    EXPECT_GT(start2, start1);
}

// Integration test: console_led_pin with time-based output
TEST(console_simulator_integration, pin_output_reflects_time_passage) {
    console_led_pin pin;

    // First output
    pin.set(true);
    std::string output1 = pin.get_last_output();

    // Wait and generate second output
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    pin.set(false);
    std::string output2 = pin.get_last_output();

    // Both outputs should exist
    EXPECT_FALSE(output1.empty());
    EXPECT_FALSE(output2.empty());

    // Outputs should be different (different timestamps and states)
    EXPECT_NE(output1, output2);
}

// Integration test: timer and pin together
TEST(console_simulator_integration, timer_and_pin_work_together) {
    real_time_timer timer;
    console_led_pin pin;

    // Reset both to sync
    timer.reset();
    pin.reset_time();

    // Let time pass
    std::this_thread::sleep_for(std::chrono::milliseconds(25));

    // Timer should show elapsed time
    uint32_t elapsed = timer.millis();
    EXPECT_GE(elapsed, 20);

    // Pin timestamp should be similar
    uint32_t pin_time = pin.get_current_timestamp_ms();
    EXPECT_GE(pin_time, 20);

    // Should be within reasonable tolerance (±10ms)
    EXPECT_LE(std::abs(static_cast<int>(elapsed) - static_cast<int>(pin_time)), 10);
}
