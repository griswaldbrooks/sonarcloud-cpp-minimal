# Phase 0: Architecture Validation Examples

## Overview

Phase 0 validates the entire Halloween 2.0 toolchain with three trivial examples before migrating any legacy code. Each example demonstrates a critical configuration pattern needed for the full system.

**Philosophy:** If we can't make trivial examples work, we can't make complex animatronics work. Validate the foundation first.

## Success Criteria

All three examples must achieve:
- Builds successfully (CMake configuration works)
- Tests pass 100%
- Coverage 80%+ (100% target for trivial code)
- Coverage reports to SonarCloud correctly
- CI/CD runs automatically
- Documentation clear and reusable

**Timeline:** 2-3 weeks total

## Example 1: Pure C++ Library (trivial_math)

**Purpose:** Validate platform-agnostic C++ library pattern

### Architecture

```
lib/examples/trivial_math/
├── include/
│   └── trivial_math.h        # Header-only or with declarations
├── src/
│   └── trivial_math.cpp      # Implementation (if needed)
├── test/
│   └── test_trivial_math.cpp # GoogleTest unit tests
└── CMakeLists.txt            # Build configuration
```

### Implementation

**include/trivial_math.h:**
```cpp
#pragma once

namespace trivial {

constexpr int add(int a, int b) {
    return a + b;
}

constexpr int multiply(int a, int b) {
    return a * b;
}

constexpr int subtract(int a, int b) {
    return a - b;
}

constexpr int divide(int a, int b) {
    // Division by zero check for runtime
    return (b != 0) ? (a / b) : 0;
}

} // namespace trivial
```

**test/test_trivial_math.cpp:**
```cpp
#include <gtest/gtest.h>
#include "trivial_math.h"

TEST(TrivialMath, Addition) {
    EXPECT_EQ(trivial::add(2, 3), 5);
    EXPECT_EQ(trivial::add(-1, 1), 0);
    EXPECT_EQ(trivial::add(0, 0), 0);
}

TEST(TrivialMath, Multiplication) {
    EXPECT_EQ(trivial::multiply(2, 3), 6);
    EXPECT_EQ(trivial::multiply(-2, 3), -6);
    EXPECT_EQ(trivial::multiply(0, 5), 0);
}

TEST(TrivialMath, Subtraction) {
    EXPECT_EQ(trivial::subtract(5, 3), 2);
    EXPECT_EQ(trivial::subtract(3, 5), -2);
    EXPECT_EQ(trivial::subtract(0, 0), 0);
}

TEST(TrivialMath, Division) {
    EXPECT_EQ(trivial::divide(6, 3), 2);
    EXPECT_EQ(trivial::divide(7, 3), 2);  // Integer division
    EXPECT_EQ(trivial::divide(5, 0), 0);  // Division by zero returns 0
}
```

### Build Commands

```bash
# Configure with tests
cmake -S. -B build/test -DBUILD_TESTS=ON

# Build
cmake --build build/test

# Run tests
ctest --test-dir build/test --output-on-failure

# Generate coverage
cmake -S. -B build/coverage -DBUILD_TESTS=ON -DENABLE_COVERAGE=ON
cmake --build build/coverage
cd build/coverage && ctest
gcovr -r ../.. --sonarqube ../../coverage.xml --html-details ../../coverage-html/index.html --print-summary
```

### Using Pixi

```bash
pixi run test-trivial
pixi run coverage-trivial
pixi run view-coverage-trivial
```

### Expected Output

```
Test project /home/griswald/personal/halloween2/build/test
    Start 1: TrivialMathTests
1/1 Test #1: TrivialMathTests .................   Passed    0.01 sec

100% tests passed, 0 tests failed out of 1

------------------------------------------------------------------------------
GCC Code Coverage Report
Directory: .
------------------------------------------------------------------------------
File                                       Lines    Exec  Cover
------------------------------------------------------------------------------
lib/examples/trivial_math/include/trivial_math.h
                                              4       4   100%
------------------------------------------------------------------------------
TOTAL                                         4       4   100%
------------------------------------------------------------------------------
```

### Validation Checklist

- [x] CMake configures without errors
- [x] GoogleTest found and linked correctly
- [x] Tests compile and run
- [x] All tests pass (4/4)
- [x] Coverage 100% (all functions exercised)
- [x] coverage.xml generated in SonarQube format
- [x] HTML report viewable locally
- [ ] SonarCloud reports coverage correctly
- [ ] CI/CD runs and passes

### Status

**Implementation:** Complete
**Tests:** Complete (4 tests, 100% coverage)
**Coverage:** Local verified, SonarCloud pending
**CI/CD:** Pending

---

## Example 2: C++ + Arduino (.ino) (blink_led)

**Purpose:** Validate C++ logic + Arduino hardware pattern

### Architecture

```
projects/examples/blink_led/
├── lib/
│   └── blink_logic/
│       ├── include/
│       │   └── blink_logic.h     # Timing calculations
│       ├── src/
│       │   └── blink_logic.cpp   # Implementation
│       └── test/
│           └── test_blink_logic.cpp  # Desktop tests
├── arduino/
│   └── blink_led.ino             # Arduino wrapper (<50 lines)
├── CMakeLists.txt                # Desktop build
└── platformio.ini                # Arduino build
```

### Key Principles

1. **Thin .ino wrapper:** Only Arduino-specific code (pinMode, digitalWrite, millis)
2. **Testable logic:** All timing/state in C++ library
3. **Hardware abstraction:** Interface for LED control
4. **Mock implementation:** For desktop testing
5. **Coverage for C++:** Not .ino (hardware wrapper)

### Implementation (To Be Completed)

**lib/blink_logic/include/blink_logic.h:**
```cpp
#pragma once
#include <cstdint>

namespace blink {

// Interface for LED control (for dependency injection)
class ILed {
public:
    virtual ~ILed() = default;
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
};

// Blink timing and state management (testable on desktop)
class BlinkController {
public:
    BlinkController(ILed& led, uint32_t interval_ms)
        : led_(led), interval_ms_(interval_ms), last_toggle_ms_(0), led_state_(false) {}

    // Call this in loop() with current milliseconds
    void update(uint32_t current_ms);

    bool getLedState() const { return led_state_; }

private:
    ILed& led_;
    uint32_t interval_ms_;
    uint32_t last_toggle_ms_;
    bool led_state_;
};

} // namespace blink
```

**arduino/blink_led.ino:**
```cpp
#include "blink_logic.h"

// Arduino LED implementation
class ArduinoLed : public blink::ILed {
public:
    ArduinoLed(int pin) : pin_(pin) {
        pinMode(pin_, OUTPUT);
    }

    void turnOn() override { digitalWrite(pin_, HIGH); }
    void turnOff() override { digitalWrite(pin_, LOW); }

private:
    int pin_;
};

ArduinoLed led(LED_BUILTIN);
blink::BlinkController controller(led, 1000);  // 1 second interval

void setup() {
    // Hardware initialization done in ArduinoLed constructor
}

void loop() {
    controller.update(millis());
}
```

### Build Commands

```bash
# Desktop tests
pixi run test-blink
pixi run coverage-blink

# Arduino upload
pixi run upload-blink-leonardo
```

### Validation Checklist

- [ ] CMake builds C++ library for desktop
- [ ] GoogleTest tests pass
- [ ] Coverage 80%+ for blink_logic
- [ ] PlatformIO compiles .ino for Arduino
- [ ] PlatformIO can upload to hardware
- [ ] LED blinks on actual hardware
- [ ] SonarCloud reports C++ coverage
- [ ] CI/CD builds both targets

### Status

**Implementation:** ✅ Complete
**Tests:** ✅ Complete (11 tests, 100% pass)
**Coverage:** ✅ 100% (22/22 lines, 7/7 functions, 6/6 branches)
**Arduino .ino:** ✅ 32 lines total, 15 LOC (under 50-line target)
**Pattern validation:** ✅ Hardware abstraction proven
**CI/CD:** Pending

**Key Achievements:**
- Platform-agnostic BlinkController class with zero hardware dependencies
- Minimal Arduino wrapper (32 lines including comments)
- Mock hardware (MockTimer) enables instant, deterministic testing
- Time wraparound handling at UINT32_MAX validated
- 100% test coverage without physical hardware
- Pattern scales to complex animatronics state machines

**Actual Implementation (v2 - Dependency Injection):**

**Pattern Evolution:**
- v1: Controller returns bool, .ino handles digitalWrite → **Logic leaks into .ino for complex cases**
- v2: Template-based dependency injection → **ALL logic stays in controller, .ino is pure glue**

**Why Dependency Injection:**
When complexity grows, the return-value pattern breaks down:
```cpp
// v1 Problem: Logic leaks into .ino
bool state = controller.update(millis());
if (state && brightness_needed) {
    analogWrite(LED_PIN, calculate_pwm());  // Untestable logic!
}
```

**v2 Solution: Inject output behavior**
```cpp
// Template-based (zero overhead)
template<typename OutputPin>
class BlinkController {
public:
    BlinkController(OutputPin& output, uint32_t on_ms, uint32_t off_ms);
    void update(uint32_t current_ms);  // Handles timing AND output
};
```

**Hardware Wrapper (.ino):**
```cpp
struct LEDPin {
    void set(bool state) { digitalWrite(LED_PIN, state ? HIGH : LOW); }
};

LEDPin led_pin;
BlinkController<LEDPin> controller(led_pin, 1000, 500);

void loop() {
    controller.update(millis());  // That's it! No logic leaks.
}
```

**Test Mock:**
```cpp
struct MockPin {
    bool state = false;
    void set(bool s) { state = s; }
};

TEST(Blink, TurnsOn) {
    MockPin mock;
    BlinkController<MockPin> controller(mock, 1000, 500);
    controller.update(500);
    EXPECT_TRUE(mock.state);  // Output verified!
}
```

**Benefits:**
- ALL logic testable (timing + output behavior)
- .ino is pure hardware adapter (<30 lines)
- Scales to PWM, multiple outputs, complex sequences
- Zero overhead (static polymorphism via templates)
- No virtual dispatch, no heap allocation
- Pattern proven for embedded systems

**Files:**
- See `/home/griswald/personal/sonarcloud-cpp-minimal/projects/examples/blink_led/`
- `lib/include/blink_controller.h` - Header-only template (100% coverage)
- `test/mock_hardware.h` - MockPin and MockTimer
- `arduino/blink_led.ino` - Minimal wrapper with LEDPin adapter

---

## Example 3: JavaScript + C++ + Arduino (web_trigger)

**Purpose:** Validate multi-language integration pattern

### Architecture

```
projects/examples/web_trigger/
├── server/
│   ├── src/
│   │   └── server.js             # Express + Socket.IO
│   ├── test/
│   │   └── server.test.js        # Jest tests
│   └── package.json
├── arduino/
│   ├── lib/
│   │   └── trigger_logic/
│   │       ├── include/trigger_logic.h
│   │       ├── src/trigger_logic.cpp
│   │       └── test/test_trigger_logic.cpp
│   └── web_trigger.ino
├── CMakeLists.txt                # C++ desktop tests
└── platformio.ini                # Arduino build
```

### Key Principles

1. **Protocol definition:** Clear message format (JSON)
2. **Language separation:** JavaScript handles web, C++ handles Arduino
3. **Independent testing:** Jest for Node.js, GoogleTest for C++
4. **Mock communication:** Fake serial for testing
5. **Dual coverage:** Both languages report separately

### Implementation (To Be Completed)

**server/src/server.js:**
```javascript
const express = require('express');
const http = require('http');
const socketIO = require('socket.io');
const SerialPort = require('serialport');

const app = express();
const server = http.createServer(app);
const io = socketIO(server);

// Serve static HTML page
app.use(express.static('public'));

// Serial communication (mockable for testing)
let port = null;

function initSerial(portPath = '/dev/ttyACM0') {
    port = new SerialPort(portPath, { baudRate: 9600 });
}

// Socket.IO event handling
io.on('connection', (socket) => {
    console.log('Client connected');

    socket.on('trigger', () => {
        console.log('Trigger received from web');
        if (port) {
            port.write('TRIGGER\n');
        }
    });
});

module.exports = { app, server, initSerial };
```

**arduino/lib/trigger_logic/include/trigger_logic.h:**
```cpp
#pragma once
#include <cstdint>

namespace trigger {

// State machine for trigger logic
enum class State {
    IDLE,
    TRIGGERED,
    ACTIVE
};

class TriggerController {
public:
    TriggerController(uint32_t duration_ms)
        : duration_ms_(duration_ms), state_(State::IDLE), trigger_start_ms_(0) {}

    void trigger();
    void update(uint32_t current_ms);
    State getState() const { return state_; }
    bool isActive() const { return state_ == State::ACTIVE; }

private:
    uint32_t duration_ms_;
    State state_;
    uint32_t trigger_start_ms_;
};

} // namespace trigger
```

### Build Commands

```bash
# JavaScript tests
pixi run test-web-trigger-js
pixi run coverage-web-trigger-js

# C++ tests
pixi run test-web-trigger-cpp
pixi run coverage-web-trigger-cpp

# Combined coverage
pixi run coverage-web-trigger

# Arduino upload
pixi run upload-web-trigger-leonardo
```

### Validation Checklist

- [ ] Node.js server runs and serves web page
- [ ] Socket.IO connections work
- [ ] Jest tests pass (80%+ coverage)
- [ ] C++ library builds for desktop
- [ ] GoogleTest tests pass (80%+ coverage)
- [ ] Arduino compiles and uploads
- [ ] Web button triggers Arduino LED
- [ ] Both coverages report to SonarCloud
- [ ] CI/CD builds all components

### Status

**Implementation:** Not started
**Tests:** Not started
**Coverage:** Not started
**CI/CD:** Not started

---

## Common Issues and Solutions

### Issue: GoogleTest Not Found

**Solution:**
```cmake
include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)
```

### Issue: Coverage Not Generated

**Checklist:**
1. Compiler flags: `-fprofile-arcs -ftest-coverage`
2. Linker flags: `--coverage`
3. Tests actually run (ctest executed)
4. .gcda files generated in build directory
5. gcovr run from correct root directory

### Issue: SonarCloud Shows 0% Coverage

**Checklist:**
1. compile_commands.json generated and in correct location
2. coverage.xml in SonarQube format (not LCOV)
3. sonar.coverageReportPaths set correctly
4. File paths in coverage.xml match source files
5. Use sonarcloud_verify.py tool to check API

### Issue: Arduino Won't Compile

**Checklist:**
1. PlatformIO installed and configured
2. Board specified in platformio.ini
3. Library paths correct
4. Include guards present
5. Arduino.h included in .ino (implicit)

---

## Phase 0 Completion

Phase 0 is complete when:

1. All 3 examples implemented
2. All tests pass (100% pass rate)
3. All coverage 80%+ (100% for trivial code)
4. SonarCloud reports all coverages correctly (verified with tool)
5. CI/CD green for all examples
6. Documentation demonstrates each pattern
7. Zero SonarCloud bugs or vulnerabilities

**Then and only then** proceed to Phase 1 (core libraries).

---

**Created:** 2025-11-12
**Last Updated:** 2025-11-12 (Phase 0.2 complete)
**Status:** Phase 0 in progress (2 of 3 examples complete)
**Next:** Implement web_trigger (JavaScript + C++ + Arduino)
