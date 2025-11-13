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
