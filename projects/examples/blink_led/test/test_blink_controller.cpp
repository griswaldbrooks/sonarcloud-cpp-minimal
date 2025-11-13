#include <gtest/gtest.h>
#include "blink_controller.h"
#include "mock_hardware.h"

class BlinkControllerTest : public ::testing::Test {
protected:
    void SetUp() override {
        timer.reset();
    }

    MockTimer timer;
};

// Test constructor and initial state
TEST_F(BlinkControllerTest, ConstructorInitializesCorrectly) {
    BlinkController controller(1000, 500);

    EXPECT_EQ(controller.getOnDuration(), 1000);
    EXPECT_EQ(controller.getOffDuration(), 500);
    EXPECT_FALSE(controller.isOn());
    EXPECT_EQ(controller.getLastToggleTime(), 0);
}

// Test initial state before any time passes
TEST_F(BlinkControllerTest, InitialStateIsOff) {
    BlinkController controller(1000, 500);

    bool state = controller.update(timer.millis());
    EXPECT_FALSE(state);
    EXPECT_FALSE(controller.isOn());
}

// Test first transition from off to on
TEST_F(BlinkControllerTest, FirstTransitionOffToOn) {
    BlinkController controller(1000, 500);

    // Initially off
    EXPECT_FALSE(controller.update(timer.millis()));

    // Still off before off_duration passes
    timer.advance(499);
    EXPECT_FALSE(controller.update(timer.millis()));

    // Should turn on after off_duration
    timer.advance(1);
    EXPECT_TRUE(controller.update(timer.millis()));
}

// Test second transition from on to off
TEST_F(BlinkControllerTest, SecondTransitionOnToOff) {
    BlinkController controller(1000, 500);

    // Get to ON state
    timer.advance(500);
    EXPECT_TRUE(controller.update(timer.millis()));

    // Should stay on before on_duration passes
    timer.advance(999);
    EXPECT_TRUE(controller.update(timer.millis()));

    // Should turn off after on_duration
    timer.advance(1);
    EXPECT_FALSE(controller.update(timer.millis()));
}

// Test multiple complete cycles
TEST_F(BlinkControllerTest, MultipleCycles) {
    BlinkController controller(1000, 500);

    // Cycle 1: Off -> On
    timer.advance(500);
    EXPECT_TRUE(controller.update(timer.millis()));

    // Cycle 1: On -> Off
    timer.advance(1000);
    EXPECT_FALSE(controller.update(timer.millis()));

    // Cycle 2: Off -> On
    timer.advance(500);
    EXPECT_TRUE(controller.update(timer.millis()));

    // Cycle 2: On -> Off
    timer.advance(1000);
    EXPECT_FALSE(controller.update(timer.millis()));

    // Cycle 3: Off -> On
    timer.advance(500);
    EXPECT_TRUE(controller.update(timer.millis()));
}

// Test reset functionality
TEST_F(BlinkControllerTest, ResetReturnsToInitialState) {
    BlinkController controller(1000, 500);

    // Get to ON state
    timer.advance(500);
    controller.update(timer.millis());
    EXPECT_TRUE(controller.isOn());

    // Reset
    controller.reset();
    timer.reset();

    // Should be back to initial OFF state
    EXPECT_FALSE(controller.isOn());
    EXPECT_EQ(controller.getLastToggleTime(), 0);

    // Should follow same pattern as initial startup
    EXPECT_FALSE(controller.update(timer.millis()));
    timer.advance(500);
    EXPECT_TRUE(controller.update(timer.millis()));
}

// Test time wraparound at UINT32_MAX
// Note: This is a complex edge case. In practice, wraparound occurs after ~49.7 days
// of continuous operation. The logic handles it correctly, but testing it requires
// careful setup of the controller state at specific times near UINT32_MAX.
// For this demonstration, we verify the wraparound calculation works with a simpler test.
TEST_F(BlinkControllerTest, HandlesTimeWraparound) {
    BlinkController controller(100, 100);

    // Get controller to a state where last_toggle is near UINT32_MAX
    // Start at UINT32_MAX - 150
    controller.update(UINT32_MAX - 150);
    // LED toggled on (elapsed from 0 is huge)

    // Update at UINT32_MAX - 40 (110ms later, should toggle off)
    controller.update(UINT32_MAX - 40);
    EXPECT_FALSE(controller.isOn());  // 110ms elapsed > 100ms on_duration

    // Now wrap around: go from UINT32_MAX - 40 to 70
    // Elapsed: (UINT32_MAX - (UINT32_MAX-40)) + 70 + 1 = 40 + 70 + 1 = 111ms
    bool state = controller.update(70);
    EXPECT_TRUE(state);  // 111ms > 100ms off_duration, should turn on
}

// Test state remains stable when called multiple times without time change
TEST_F(BlinkControllerTest, StableStateWhenTimeUnchanged) {
    BlinkController controller(1000, 500);

    // Turn LED on
    timer.advance(500);
    EXPECT_TRUE(controller.update(timer.millis()));

    // Call update multiple times with same time
    EXPECT_TRUE(controller.update(timer.millis()));
    EXPECT_TRUE(controller.update(timer.millis()));
    EXPECT_TRUE(controller.update(timer.millis()));
}

// Test with different timing configurations
TEST_F(BlinkControllerTest, DifferentTimingConfigurations) {
    // Fast blink
    BlinkController fast(100, 100);
    timer.advance(100);
    EXPECT_TRUE(fast.update(timer.millis()));
    timer.advance(100);
    EXPECT_FALSE(fast.update(timer.millis()));

    // Slow blink
    timer.reset();
    BlinkController slow(5000, 5000);
    timer.advance(5000);
    EXPECT_TRUE(slow.update(timer.millis()));
    timer.advance(5000);
    EXPECT_FALSE(slow.update(timer.millis()));

    // Asymmetric blink (long on, short off)
    timer.reset();
    BlinkController asymmetric(3000, 200);
    timer.advance(200);
    EXPECT_TRUE(asymmetric.update(timer.millis()));
    timer.advance(3000);
    EXPECT_FALSE(asymmetric.update(timer.millis()));
}

// Test edge case: zero duration (should toggle immediately)
TEST_F(BlinkControllerTest, ZeroDurationTogglesImmediately) {
    BlinkController controller(0, 0);

    // First call should turn on immediately
    EXPECT_TRUE(controller.update(timer.millis()));

    // Second call should turn off immediately
    EXPECT_FALSE(controller.update(timer.millis()));

    // Third call should turn on again
    EXPECT_TRUE(controller.update(timer.millis()));
}

// Test that LED doesn't toggle prematurely
TEST_F(BlinkControllerTest, NoEarlyToggle) {
    BlinkController controller(1000, 500);

    // Start in OFF state
    EXPECT_FALSE(controller.update(timer.millis()));

    // Advance just before toggle point
    for (uint32_t i = 1; i < 500; ++i) {
        timer.advance(1);
        EXPECT_FALSE(controller.update(timer.millis()));
    }

    // Now it should toggle
    timer.advance(1);
    EXPECT_TRUE(controller.update(timer.millis()));
}
