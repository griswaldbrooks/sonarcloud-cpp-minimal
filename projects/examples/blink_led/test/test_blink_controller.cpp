#include <gtest/gtest.h>
#include "blink_controller.h"
#include "mock_hardware.h"

class BlinkControllerTest : public ::testing::Test {
protected:
    void SetUp() override {
        timer.reset();
        mock_pin.reset();
    }

    MockTimer timer;
    MockPin mock_pin;
};

// Test constructor and initial state
TEST_F(BlinkControllerTest, ConstructorInitializesCorrectly) {
    BlinkController<MockPin> controller(mock_pin, 1000, 500);

    EXPECT_EQ(controller.getOnDuration(), 1000);
    EXPECT_EQ(controller.getOffDuration(), 500);
    EXPECT_FALSE(controller.isOn());
    EXPECT_EQ(controller.getLastToggleTime(), 0);
}

// Test initial state before any time passes
TEST_F(BlinkControllerTest, InitialStateIsOff) {
    BlinkController<MockPin> controller(mock_pin, 1000, 500);

    controller.update(timer.millis());
    EXPECT_FALSE(mock_pin.getState());
    EXPECT_FALSE(controller.isOn());
}

// Test first transition from off to on
TEST_F(BlinkControllerTest, FirstTransitionOffToOn) {
    BlinkController<MockPin> controller(mock_pin, 1000, 500);

    // Initially off
    controller.update(timer.millis());
    EXPECT_FALSE(mock_pin.getState());

    // Still off before off_duration passes
    timer.advance(499);
    controller.update(timer.millis());
    EXPECT_FALSE(mock_pin.getState());

    // Should turn on after off_duration
    timer.advance(1);
    controller.update(timer.millis());
    EXPECT_TRUE(mock_pin.getState());
}

// Test second transition from on to off
TEST_F(BlinkControllerTest, SecondTransitionOnToOff) {
    BlinkController<MockPin> controller(mock_pin, 1000, 500);

    // Get to ON state
    timer.advance(500);
    controller.update(timer.millis());
    EXPECT_TRUE(mock_pin.getState());

    // Should stay on before on_duration passes
    timer.advance(999);
    controller.update(timer.millis());
    EXPECT_TRUE(mock_pin.getState());

    // Should turn off after on_duration
    timer.advance(1);
    controller.update(timer.millis());
    EXPECT_FALSE(mock_pin.getState());
}

// Test multiple complete cycles
TEST_F(BlinkControllerTest, MultipleCycles) {
    BlinkController<MockPin> controller(mock_pin, 1000, 500);

    // Cycle 1: Off -> On
    timer.advance(500);
    controller.update(timer.millis());
    EXPECT_TRUE(mock_pin.getState());

    // Cycle 1: On -> Off
    timer.advance(1000);
    controller.update(timer.millis());
    EXPECT_FALSE(mock_pin.getState());

    // Cycle 2: Off -> On
    timer.advance(500);
    controller.update(timer.millis());
    EXPECT_TRUE(mock_pin.getState());

    // Cycle 2: On -> Off
    timer.advance(1000);
    controller.update(timer.millis());
    EXPECT_FALSE(mock_pin.getState());

    // Cycle 3: Off -> On
    timer.advance(500);
    controller.update(timer.millis());
    EXPECT_TRUE(mock_pin.getState());
}

// Test reset functionality
TEST_F(BlinkControllerTest, ResetReturnsToInitialState) {
    BlinkController<MockPin> controller(mock_pin, 1000, 500);

    // Get to ON state
    timer.advance(500);
    controller.update(timer.millis());
    EXPECT_TRUE(controller.isOn());
    EXPECT_TRUE(mock_pin.getState());

    // Reset
    controller.reset();
    timer.reset();

    // Should be back to initial OFF state
    EXPECT_FALSE(controller.isOn());
    EXPECT_FALSE(mock_pin.getState());  // Pin should be set to LOW
    EXPECT_EQ(controller.getLastToggleTime(), 0);

    // Should follow same pattern as initial startup
    controller.update(timer.millis());
    EXPECT_FALSE(mock_pin.getState());
    timer.advance(500);
    controller.update(timer.millis());
    EXPECT_TRUE(mock_pin.getState());
}

// Test time wraparound at UINT32_MAX
// Note: This is a complex edge case. In practice, wraparound occurs after ~49.7 days
// of continuous operation. The logic handles it correctly, but testing it requires
// careful setup of the controller state at specific times near UINT32_MAX.
// For this demonstration, we verify the wraparound calculation works with a simpler test.
TEST_F(BlinkControllerTest, HandlesTimeWraparound) {
    BlinkController<MockPin> controller(mock_pin, 100, 100);

    // Get controller to a state where last_toggle is near UINT32_MAX
    // Start at UINT32_MAX - 150
    controller.update(UINT32_MAX - 150);
    // LED toggled on (elapsed from 0 is huge)

    // Update at UINT32_MAX - 40 (110ms later, should toggle off)
    controller.update(UINT32_MAX - 40);
    EXPECT_FALSE(controller.isOn());  // 110ms elapsed > 100ms on_duration
    EXPECT_FALSE(mock_pin.getState());

    // Now wrap around: go from UINT32_MAX - 40 to 70
    // Elapsed: (UINT32_MAX - (UINT32_MAX-40)) + 70 + 1 = 40 + 70 + 1 = 111ms
    controller.update(70);
    EXPECT_TRUE(controller.isOn());  // 111ms > 100ms off_duration, should turn on
    EXPECT_TRUE(mock_pin.getState());
}

// Test state remains stable when called multiple times without time change
TEST_F(BlinkControllerTest, StableStateWhenTimeUnchanged) {
    BlinkController<MockPin> controller(mock_pin, 1000, 500);

    // Turn LED on
    timer.advance(500);
    controller.update(timer.millis());
    EXPECT_TRUE(mock_pin.getState());

    // Call update multiple times with same time
    controller.update(timer.millis());
    EXPECT_TRUE(mock_pin.getState());
    controller.update(timer.millis());
    EXPECT_TRUE(mock_pin.getState());
    controller.update(timer.millis());
    EXPECT_TRUE(mock_pin.getState());
}

// Test with different timing configurations
TEST_F(BlinkControllerTest, DifferentTimingConfigurations) {
    // Fast blink
    BlinkController<MockPin> fast(mock_pin, 100, 100);
    timer.advance(100);
    fast.update(timer.millis());
    EXPECT_TRUE(mock_pin.getState());
    timer.advance(100);
    fast.update(timer.millis());
    EXPECT_FALSE(mock_pin.getState());

    // Slow blink
    timer.reset();
    mock_pin.reset();
    BlinkController<MockPin> slow(mock_pin, 5000, 5000);
    timer.advance(5000);
    slow.update(timer.millis());
    EXPECT_TRUE(mock_pin.getState());
    timer.advance(5000);
    slow.update(timer.millis());
    EXPECT_FALSE(mock_pin.getState());

    // Asymmetric blink (long on, short off)
    timer.reset();
    mock_pin.reset();
    BlinkController<MockPin> asymmetric(mock_pin, 3000, 200);
    timer.advance(200);
    asymmetric.update(timer.millis());
    EXPECT_TRUE(mock_pin.getState());
    timer.advance(3000);
    asymmetric.update(timer.millis());
    EXPECT_FALSE(mock_pin.getState());
}

// Test edge case: zero duration (should toggle immediately)
TEST_F(BlinkControllerTest, ZeroDurationTogglesImmediately) {
    BlinkController<MockPin> controller(mock_pin, 0, 0);

    // First call should turn on immediately
    controller.update(timer.millis());
    EXPECT_TRUE(mock_pin.getState());

    // Second call should turn off immediately
    controller.update(timer.millis());
    EXPECT_FALSE(mock_pin.getState());

    // Third call should turn on again
    controller.update(timer.millis());
    EXPECT_TRUE(mock_pin.getState());
}

// Test that LED doesn't toggle prematurely
TEST_F(BlinkControllerTest, NoEarlyToggle) {
    BlinkController<MockPin> controller(mock_pin, 1000, 500);

    // Start in OFF state
    controller.update(timer.millis());
    EXPECT_FALSE(mock_pin.getState());

    // Advance just before toggle point
    for (uint32_t i = 1; i < 500; ++i) {
        timer.advance(1);
        controller.update(timer.millis());
        EXPECT_FALSE(mock_pin.getState());
    }

    // Now it should toggle
    timer.advance(1);
    controller.update(timer.millis());
    EXPECT_TRUE(mock_pin.getState());
}

// Test that output pin is updated on every update() call
TEST_F(BlinkControllerTest, OutputPinUpdatedEveryCall) {
    BlinkController<MockPin> controller(mock_pin, 1000, 500);
    uint32_t initial_count = mock_pin.getToggleCount();

    // Each update() should call set() on the pin
    controller.update(timer.millis());
    EXPECT_GT(mock_pin.getToggleCount(), initial_count);

    uint32_t count_after_first = mock_pin.getToggleCount();
    controller.update(timer.millis());
    EXPECT_GT(mock_pin.getToggleCount(), count_after_first);
}
