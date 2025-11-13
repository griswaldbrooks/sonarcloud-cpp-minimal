# Blink LED Example

**Phase 0.2: Platform-Agnostic C++ + Arduino Pattern**

This example demonstrates the production embedded systems pattern used throughout the halloween2 project:
- Platform-agnostic C++ logic library
- Minimal Arduino .ino wrapper (<50 lines)
- Desktop testing with GoogleTest
- 80%+ test coverage
- Hardware abstraction via interfaces

## Pattern Overview

```
┌─────────────────────────────────────────────────┐
│                 Application                      │
│            (Arduino .ino or Desktop)             │
└───────────────────┬─────────────────────────────┘
                    │
        ┌───────────┴──────────────┐
        │                          │
┌───────▼──────────┐    ┌──────────▼─────────┐
│  Hardware Layer  │    │   Mock Hardware    │
│  (Arduino APIs)  │    │  (Testing Only)    │
└───────┬──────────┘    └──────────┬─────────┘
        │                          │
        └───────────┬──────────────┘
                    │
        ┌───────────▼──────────────┐
        │   BlinkController         │
        │   (Platform-Agnostic)     │
        │   - Pure C++ logic        │
        │   - No hardware deps      │
        │   - Fully testable        │
        └───────────────────────────┘
```

## Project Structure

```
blink_led/
├── lib/                          # Platform-agnostic library
│   ├── include/
│   │   └── blink_controller.h    # Public API
│   └── src/
│       └── blink_controller.cpp  # Implementation
├── arduino/
│   └── blink_led.ino             # Arduino wrapper (27 lines)
├── test/
│   ├── test_blink_controller.cpp # GoogleTest tests
│   └── mock_hardware.h           # Mock timer for testing
├── CMakeLists.txt                # Build configuration
└── README.md                     # This file
```

## Key Features

### 1. Platform-Agnostic Logic
The `BlinkController` class contains all timing logic with zero hardware dependencies:
- Takes current time as parameter (not calling `millis()` directly)
- Returns desired LED state (not calling `digitalWrite()`)
- Pure C++ with no Arduino-specific code

### 2. Minimal Arduino Wrapper
The .ino file is just 27 lines:
- `setup()`: Initialize hardware
- `loop()`: Read time, update controller, write output
- No business logic - just hardware glue code

### 3. Comprehensive Testing
13 test cases covering:
- Initial state verification
- State transitions (off → on → off)
- Multiple complete cycles
- Reset functionality
- Time wraparound at UINT32_MAX
- Edge cases (zero duration, stable state)
- Different timing configurations

### 4. High Coverage
Target: 80%+ line coverage (aiming for 100%)
- All logic paths tested
- Edge cases covered
- Mock hardware for time simulation

## Building and Testing

### Desktop Tests
```bash
# Configure
cmake -B build -DBUILD_TESTS=ON -DENABLE_COVERAGE=ON

# Build
cmake --build build

# Run tests
./build/projects/examples/blink_led/test_blink_controller

# Generate coverage
cd build
make coverage-blink

# View coverage report
xdg-open coverage-blink.html
```

### Arduino Build
```bash
# Using arduino-cli
arduino-cli compile --fqbn arduino:avr:leonardo projects/examples/blink_led/arduino/

# Upload to board
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:leonardo projects/examples/blink_led/arduino/
```

## Design Rationale

### Why Separate Logic from Hardware?

1. **Testability**: Business logic runs on desktop without hardware
2. **Speed**: Tests run instantly (no delays, no flashing)
3. **Coverage**: Can achieve 80%+ coverage with desktop tests
4. **Portability**: Same logic works on any platform
5. **Maintainability**: Logic changes don't require hardware to verify

### Why Keep .ino Minimal?

1. **Arduino IDE limitations**: Hard to test .ino code
2. **Compilation speed**: Less Arduino code = faster builds
3. **Clarity**: Easy to see what the hardware interface is
4. **Best practice**: .ino should just be "main()" + hardware

### Why Mock Hardware?

1. **Time control**: Simulate hours in milliseconds
2. **Determinism**: No race conditions or timing issues
3. **Edge cases**: Test wraparound, exact timing boundaries
4. **CI/CD**: Tests run in any environment

## Test Coverage Examples

### Initial State Test
```cpp
TEST_F(BlinkControllerTest, InitialStateIsOff) {
    BlinkController controller(1000, 500);
    bool state = controller.update(timer.millis());
    EXPECT_FALSE(state);
}
```

### State Transition Test
```cpp
TEST_F(BlinkControllerTest, FirstTransitionOffToOn) {
    BlinkController controller(1000, 500);

    EXPECT_FALSE(controller.update(timer.millis()));  // Initially off
    timer.advance(499);                                // Just before toggle
    EXPECT_FALSE(controller.update(timer.millis()));  // Still off
    timer.advance(1);                                  // Reach threshold
    EXPECT_TRUE(controller.update(timer.millis()));   // Now on
}
```

### Wraparound Test
```cpp
TEST_F(BlinkControllerTest, HandlesTimeWraparound) {
    BlinkController controller(1000, 500);

    timer.setTime(UINT32_MAX - 100);
    controller.update(timer.millis());
    EXPECT_FALSE(controller.isOn());

    timer.setTime(400);  // Wrapped around, total elapsed = 501ms
    EXPECT_TRUE(controller.update(timer.millis()));  // Toggled
}
```

## Extending This Pattern

This pattern scales to complex animatronics:

### State Machine Example
```cpp
class AnimatronicsController {
public:
    enum State { IDLE, HATCHING, ACTIVE, RESETTING };

    State update(uint32_t current_time_ms, bool trigger);

private:
    State current_state_;
    uint32_t state_start_time_;
};
```

### Multi-Servo Example
```cpp
class ServoSequenceController {
public:
    void update(uint32_t current_time_ms);
    std::vector<ServoCommand> getServoCommands() const;

private:
    SequenceState state_;
    std::vector<Keyframe> keyframes_;
};
```

### Hardware Interface Example
```cpp
class IServoController {
public:
    virtual void setPosition(uint8_t servo_id, uint16_t position) = 0;
    virtual uint16_t getPosition(uint8_t servo_id) const = 0;
};

class PCA9685ServoController : public IServoController {
    // Real hardware implementation
};

class MockServoController : public IServoController {
    // Testing implementation
};
```

## Success Metrics

- ✅ 27 lines in .ino file (target: <50)
- ✅ 13 comprehensive test cases
- ✅ 100% line coverage (target: 80%+)
- ✅ All tests pass
- ✅ Zero hardware dependencies in logic
- ✅ Compiles for Arduino and desktop

## Lessons Applied

From halloween1 project (2024):
1. Test coverage prevents regressions
2. Hardware abstraction enables testing
3. Minimal .ino files are maintainable
4. Mock hardware makes tests fast
5. CI/CD catches issues early

## Next Steps

Phase 0.3 will add:
- Multi-file C++ example
- Servo control interfaces
- State machine pattern
- Integration test examples

## References

- **halloween1**: `/home/griswald/personal/halloween/hatching_egg` (92% C++ coverage)
- **halloween1**: `/home/griswald/personal/halloween/window_spider_trigger` (needs improvement)
- **GoogleTest**: https://github.com/google/googletest
- **Arduino CLI**: https://arduino.github.io/arduino-cli/

---

**Created:** 2025-11-12
**Pattern:** Platform-Agnostic C++ + Minimal Arduino Wrapper
**Coverage:** 100% (13 tests)
**Status:** ✅ Complete
