#include <iostream>
#include <thread>
#include <chrono>
#include <cstdint>
#include "blink_controller.h"
#include "console_simulator.h"

/**
 * @brief Run blink controller demo with console output
 *
 * Simple demonstration runner that uses console_simulator components
 * to visualize the blink_controller in action.
 *
 * This function is kept as a thin wrapper - all testable logic is in
 * console_simulator.h and blink_controller.h libraries.
 */
void run_demo() {
    // Configuration
    constexpr uint32_t ON_DURATION_MS = 1000;
    constexpr uint32_t OFF_DURATION_MS = 500;
    constexpr uint32_t SIMULATION_DURATION_MS = 10000;
    constexpr uint32_t UPDATE_INTERVAL_MS = 50;

    // Create components (all from libraries)
    console_led_pin console_pin;
    real_time_timer timer;
    blink_controller<console_led_pin> controller(console_pin, ON_DURATION_MS, OFF_DURATION_MS);

    // Print header
    std::cout << "\n=== blink_controller Demo ===" << std::endl;
    std::cout << "Demonstrating dependency injection with console_led_pin\n" << std::endl;
    std::cout << "Configuration:" << std::endl;
    std::cout << "  ON duration:  " << ON_DURATION_MS << "ms" << std::endl;
    std::cout << "  OFF duration: " << OFF_DURATION_MS << "ms" << std::endl;
    std::cout << "  Total cycle:  " << (ON_DURATION_MS + OFF_DURATION_MS) << "ms" << std::endl;
    std::cout << "\nRunning for 10 seconds...\n" << std::endl;

    // Synchronize timers
    timer.reset();
    console_pin.reset_time();

    // Main demo loop
    while (timer.millis() < SIMULATION_DURATION_MS) {
        controller.update(timer.millis());
        std::cout << console_pin.get_last_output() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(UPDATE_INTERVAL_MS));
    }

    // Print footer
    std::cout << "\n=== Demo Complete ===" << std::endl;
    std::cout << "Notice how the controller manages timing and state transitions" << std::endl;
    std::cout << "while console_led_pin handles the output presentation." << std::endl;
    std::cout << "\nThis demonstrates the power of dependency injection:" << std::endl;
    std::cout << "  - Same blink_controller logic works with different output types" << std::endl;
    std::cout << "  - mock_pin for testing, console_led_pin for demo, hardware pins for production" << std::endl;
    std::cout << "  - Zero runtime overhead (template-based static polymorphism)" << std::endl;
    std::cout << "  - 100% testable business logic\n" << std::endl;
}

int main() {
    run_demo();
    return 0;
}
