#include "blink_controller.h"

BlinkController::BlinkController(uint32_t on_duration_ms, uint32_t off_duration_ms)
    : on_duration_ms_(on_duration_ms),
      off_duration_ms_(off_duration_ms),
      last_toggle_time_ms_(0),
      led_on_(false) {}

bool BlinkController::update(uint32_t current_time_ms) {
    // Calculate time elapsed since last toggle
    uint32_t elapsed;
    if (current_time_ms >= last_toggle_time_ms_) {
        // Normal case: no wraparound
        elapsed = current_time_ms - last_toggle_time_ms_;
    } else {
        // Handle uint32_t wraparound (occurs after ~49.7 days)
        elapsed = (UINT32_MAX - last_toggle_time_ms_) + current_time_ms + 1;
    }

    // Determine if we should toggle
    uint32_t target_duration = led_on_ ? on_duration_ms_ : off_duration_ms_;

    if (elapsed >= target_duration) {
        // Toggle the LED state
        led_on_ = !led_on_;
        last_toggle_time_ms_ = current_time_ms;
    }

    return led_on_;
}

void BlinkController::reset() {
    last_toggle_time_ms_ = 0;
    led_on_ = false;
}
