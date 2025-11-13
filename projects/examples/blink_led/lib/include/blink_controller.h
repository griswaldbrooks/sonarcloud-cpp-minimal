#pragma once
#include <cstdint>

/**
 * @brief Platform-agnostic LED blink timing controller
 *
 * Controls LED on/off timing without any hardware dependencies.
 * Suitable for desktop testing and embedded deployment.
 */
class BlinkController {
public:
    /**
     * @brief Construct a new Blink Controller
     *
     * @param on_duration_ms How long LED stays on (milliseconds)
     * @param off_duration_ms How long LED stays off (milliseconds)
     */
    BlinkController(uint32_t on_duration_ms, uint32_t off_duration_ms);

    /**
     * @brief Update LED state based on current time
     *
     * Call this in your main loop with the current time.
     * Handles state transitions automatically.
     *
     * @param current_time_ms Current time in milliseconds
     * @return true LED should be on
     * @return false LED should be off
     */
    bool update(uint32_t current_time_ms);

    /**
     * @brief Reset controller to initial state
     *
     * LED will be off, timer will be reset to 0.
     */
    void reset();

    // Getters for testing and state inspection
    uint32_t getOnDuration() const { return on_duration_ms_; }
    uint32_t getOffDuration() const { return off_duration_ms_; }
    bool isOn() const { return led_on_; }
    uint32_t getLastToggleTime() const { return last_toggle_time_ms_; }

private:
    uint32_t on_duration_ms_;
    uint32_t off_duration_ms_;
    uint32_t last_toggle_time_ms_;
    bool led_on_;
};
