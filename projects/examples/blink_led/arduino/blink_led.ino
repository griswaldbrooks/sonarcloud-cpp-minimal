/**
 * @file blink_led.ino
 * @brief Minimal Arduino wrapper for BlinkController
 *
 * Demonstrates the C++ + Arduino pattern:
 * - All logic in platform-agnostic C++ library
 * - Arduino .ino is just a thin hardware wrapper
 * - Business logic fully testable on desktop
 */

#include <Arduino.h>
#include "../lib/include/blink_controller.h"

// Hardware configuration
const int LED_PIN = 13;           // Built-in LED on most Arduino boards

// Timing configuration (milliseconds)
const uint32_t ON_DURATION_MS = 1000;   // LED on for 1 second
const uint32_t OFF_DURATION_MS = 500;   // LED off for 0.5 seconds

// Platform-agnostic controller
BlinkController controller(ON_DURATION_MS, OFF_DURATION_MS);

void setup() {
    pinMode(LED_PIN, OUTPUT);
    controller.reset();
}

void loop() {
    bool led_state = controller.update(millis());
    digitalWrite(LED_PIN, led_state ? HIGH : LOW);
}
