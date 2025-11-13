/**
 * @file blink_led.ino
 * @brief Minimal Arduino wrapper for BlinkController (Dependency Injection Pattern)
 *
 * Demonstrates the C++ + Arduino pattern with dependency injection:
 * - All logic (timing AND output) in platform-agnostic C++ library
 * - Arduino .ino is just hardware pin adapter (< 30 lines!)
 * - Business logic 100% testable on desktop
 * - Zero overhead (templates compile to same code as direct calls)
 *
 * Pattern Benefits:
 * - No conditional logic in .ino (can't test conditionals without hardware)
 * - Controller handles ALL behavior (timing, state, output)
 * - Easy to extend (PWM, multiple LEDs, complex sequences)
 * - Scales to complex animatronics without .ino bloat
 */

#include <Arduino.h>
#include "../lib/include/blink_controller.h"

// Hardware configuration
const int LED_PIN = 13;           // Built-in LED on most Arduino boards

// Timing configuration (milliseconds)
const uint32_t ON_DURATION_MS = 1000;   // LED on for 1 second
const uint32_t OFF_DURATION_MS = 500;   // LED off for 0.5 seconds

/**
 * @brief Hardware pin adapter for dependency injection
 *
 * Adapts Arduino digitalWrite to the pin interface expected by BlinkController.
 * This is the ONLY hardware-specific code in the entire blink system!
 */
struct LEDPin {
    void set(bool state) {
        digitalWrite(LED_PIN, state ? HIGH : LOW);
    }
};

// Hardware pin instance
LEDPin led_pin;

// Platform-agnostic controller with injected pin
BlinkController<LEDPin> controller(led_pin, ON_DURATION_MS, OFF_DURATION_MS);

void setup() {
    pinMode(LED_PIN, OUTPUT);
    controller.reset();
}

void loop() {
    // That's it! Controller handles timing AND output.
    // No logic here = nothing to test on hardware!
    controller.update(millis());
}
