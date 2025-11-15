#pragma once
#include <chrono>
#include <string>
#include <sstream>
#include <cstdint>

/**
 * @brief Console output pin with colored terminal display
 *
 * Implements the same interface as mock_pin but outputs visual feedback
 * to the terminal with ANSI color codes. This class is testable by
 * providing methods to retrieve formatted output without actual console I/O.
 *
 * Design:
 * - State tracking (ON/OFF)
 * - Timestamp management (milliseconds since start)
 * - ANSI color formatting (configurable)
 * - Output string generation (testable without console)
 *
 * Usage in tests:
 *   console_led_pin pin;
 *   pin.set(true);
 *   std::string output = pin.get_last_output();
 *   EXPECT_THAT(output, HasSubstr("ON"));
 *
 * Usage in production (main.cpp):
 *   console_led_pin pin;
 *   pin.set(true);
 *   std::cout << pin.get_last_output() << std::endl;
 */
struct console_led_pin {
public:
    /**
     * @brief Set LED state and generate formatted output
     *
     * @param state true for ON (green), false for OFF (red)
     */
    void set(bool state) {
        state_ = state;

        // Get current timestamp
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - start_time_);

        // Generate formatted output string (testable!)
        last_output_ = format_output(duration.count(), state_);
    }

    /**
     * @brief Get current LED state
     *
     * @return true LED is ON
     * @return false LED is OFF
     */
    bool get_state() const {
        return state_;
    }

    /**
     * @brief Reset the start time for timestamp display
     */
    void reset_time() {
        start_time_ = std::chrono::steady_clock::now();
    }

    /**
     * @brief Get the last formatted output (for testing and display)
     *
     * @return std::string Formatted output with timestamp and state
     */
    std::string get_last_output() const {
        return last_output_;
    }

    /**
     * @brief Get current timestamp in milliseconds
     *
     * @return uint32_t Milliseconds since start_time_
     */
    uint32_t get_current_timestamp_ms() const {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - start_time_);
        return static_cast<uint32_t>(duration.count());
    }

    /**
     * @brief Format output string with ANSI colors (testable)
     *
     * This is the core formatting logic that can be tested without
     * actual console output.
     *
     * @param timestamp_ms Timestamp in milliseconds
     * @param state LED state (true=ON, false=OFF)
     * @return std::string Formatted output string
     */
    static std::string format_output(uint32_t timestamp_ms, bool state) {
        std::ostringstream oss;

        // ANSI color codes
        char const* green = "\033[32m";  // Green text
        char const* red = "\033[31m";    // Red text
        char const* reset = "\033[0m";   // Reset to default

        oss << "[" << timestamp_ms << "ms] LED: ";

        if (state) {
            oss << green << "███ ON ███" << reset;
        } else {
            oss << red << "▓▓▓ OFF ▓▓▓" << reset;
        }

        return oss.str();
    }

    /**
     * @brief Strip ANSI color codes from string (for testing)
     *
     * Removes ANSI escape sequences to make testing easier.
     *
     * @param input String potentially containing ANSI codes
     * @return std::string String with ANSI codes removed
     */
    static std::string strip_ansi_codes(std::string const& input) {
        std::string result;
        bool in_escape = false;

        for (char c : input) {
            if (c == '\033') {
                in_escape = true;
            } else if (in_escape && c == 'm') {
                in_escape = false;
            } else if (!in_escape) {
                result += c;
            }
        }

        return result;
    }

private:
    bool state_ = false;
    std::chrono::steady_clock::time_point start_time_ =
        std::chrono::steady_clock::now();
    std::string last_output_;
};

/**
 * @brief Simple timer that returns real-world milliseconds
 *
 * Provides the same interface as mock_timer but uses actual system time.
 * Testable by verifying monotonic behavior and reset functionality.
 *
 * Design:
 * - Uses std::chrono::steady_clock (monotonic, never goes backwards)
 * - Provides millisecond precision
 * - Supports reset to zero
 * - Testable by verifying behavior over small time intervals
 *
 * Testing Strategy:
 * - Cannot test exact time values (system-dependent)
 * - CAN test: monotonic increase, reset behavior, reasonable bounds
 * - Focus on interface compliance, not absolute timing
 */
struct real_time_timer {
public:
    real_time_timer() : start_time_(std::chrono::steady_clock::now()) {}

    /**
     * @brief Get milliseconds elapsed since timer creation or last reset
     *
     * @return uint32_t Elapsed time in milliseconds
     */
    uint32_t millis() const {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - start_time_);
        return static_cast<uint32_t>(duration.count());
    }

    /**
     * @brief Reset timer to zero
     */
    void reset() {
        start_time_ = std::chrono::steady_clock::now();
    }

    /**
     * @brief Get start time (for testing)
     *
     * @return std::chrono::steady_clock::time_point The timer's start time
     */
    std::chrono::steady_clock::time_point get_start_time() const {
        return start_time_;
    }

private:
    std::chrono::steady_clock::time_point start_time_;
};
