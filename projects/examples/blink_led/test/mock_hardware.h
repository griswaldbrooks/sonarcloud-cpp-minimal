#pragma once
#include <cstdint>

/**
 * @brief Mock timer for testing time-dependent logic
 *
 * Simulates millis() function from Arduino without real-time delays.
 * Allows tests to run instantly while simulating arbitrary time spans.
 */
class MockTimer {
public:
    /**
     * @brief Advance the mock time forward
     *
     * @param milliseconds Time to advance in milliseconds
     */
    void advance(uint32_t milliseconds) {
        current_time_ms_ += milliseconds;
    }

    /**
     * @brief Get current mock time
     *
     * @return uint32_t Current time in milliseconds
     */
    uint32_t millis() const {
        return current_time_ms_;
    }

    /**
     * @brief Reset timer to zero
     */
    void reset() {
        current_time_ms_ = 0;
    }

    /**
     * @brief Set absolute time (useful for edge case testing)
     *
     * @param time_ms Absolute time to set
     */
    void setTime(uint32_t time_ms) {
        current_time_ms_ = time_ms;
    }

private:
    uint32_t current_time_ms_ = 0;
};

/**
 * @brief Mock output pin for testing hardware output logic
 *
 * Simulates a digital output pin without actual hardware.
 * Captures state changes for verification in tests.
 */
class MockPin {
public:
    /**
     * @brief Set pin state
     *
     * @param state true for HIGH, false for LOW
     */
    void set(bool state) {
        state_ = state;
        toggle_count_++;
    }

    /**
     * @brief Get current pin state
     *
     * @return true Pin is HIGH
     * @return false Pin is LOW
     */
    bool getState() const {
        return state_;
    }

    /**
     * @brief Get number of times set() has been called
     *
     * Useful for testing that output changes occur as expected
     *
     * @return uint32_t Number of set() calls
     */
    uint32_t getToggleCount() const {
        return toggle_count_;
    }

    /**
     * @brief Reset pin to initial state
     */
    void reset() {
        state_ = false;
        toggle_count_ = 0;
    }

private:
    bool state_ = false;
    uint32_t toggle_count_ = 0;
};
