# Halloween 2.0 - Clean Slate Implementation

## Project Overview

Halloween 2.0 is a clean-slate rewrite of the Halloween haunted house animatronics system. This project prioritizes architecture validation before migrating any legacy code.

**Status:** Phase 0 - Architecture Validation (In Progress)

## Purpose

Build a production-ready animatronics system with:
- Modern C++ architecture (C++17+)
- Comprehensive testing (80%+ coverage goal)
- Multi-platform support (Arduino Leonardo, Nano, Beetle, NodeMCU, RPi Pico)
- Hardware abstraction for testability
- Multi-language integration (C++, JavaScript, Python)

## Phase 0: Architecture Validation

**Principle:** Validate the entire toolchain with trivial examples BEFORE migrating legacy code.

### Three Required Examples

1. **Pure C++ Library** (`lib/examples/trivial_math/`)
   - Platform-agnostic business logic
   - GoogleTest tests achieving 100% coverage
   - gcov/gcovr coverage reporting
   - No hardware dependencies
   - **Status:** In progress

2. **C++ + Arduino (.ino)** (`projects/examples/blink_led/`)
   - Minimal .ino file (<50 lines) as thin wrapper
   - C++ logic library (blink timing, state management)
   - Desktop tests for C++ logic
   - Arduino compilation for hardware deployment
   - Coverage for C++ only (not .ino)
   - **Status:** Not started

3. **JavaScript + C++ + Arduino** (`projects/examples/web_trigger/`)
   - Node.js/Express server with Socket.IO
   - C++ Arduino component
   - Serial/WebSocket communication protocol
   - Jest tests for JavaScript (80%+ coverage)
   - GoogleTest for C++ (80%+ coverage)
   - Dual-language coverage reporting to SonarCloud
   - **Status:** Not started

### Phase 0 Success Criteria

Must achieve ALL before proceeding to Phase 1:
- All 3 examples build successfully
- All tests pass (100% pass rate)
- Coverage reports correctly to SonarCloud (80%+ for each)
- CI/CD pipeline runs all examples automatically
- Documentation demonstrates each pattern clearly
- No SonarCloud bugs or vulnerabilities

**Timeline:** 2-3 weeks

## Repository Structure

```
halloween2/
├── lib/
│   ├── animatronics_core/            # Phase 1+: Core libraries
│   │   ├── state_machine/            # Reusable state machine
│   │   ├── servo_control/            # Servo abstractions
│   │   └── timing/                   # Timing utilities
│   └── examples/                     # Phase 0: Validation
│       └── trivial_math/             # Pure C++ example
│           ├── include/trivial_math.h
│           ├── src/trivial_math.cpp  # (if needed)
│           ├── test/test_trivial_math.cpp
│           └── CMakeLists.txt
├── projects/
│   └── examples/                     # Phase 0: Project patterns
│       ├── blink_led/                # C++ + .ino
│       │   ├── lib/blink_logic/      # C++ logic
│       │   ├── arduino/blink_led.ino # Thin wrapper
│       │   ├── test/                 # Desktop tests
│       │   └── CMakeLists.txt
│       └── web_trigger/              # JS + C++ + .ino
│           ├── server/               # Node.js app
│           ├── arduino/              # C++ + .ino
│           ├── test/                 # Jest + GoogleTest
│           └── CMakeLists.txt
├── toolchains/
│   ├── Desktop.cmake                 # Native x86_64
│   └── Arduino-leonardo.cmake        # Arduino Leonardo target
├── docs/
│   ├── CLEAN_SLATE_IMPLEMENTATION_PLAN.md  # Full roadmap
│   ├── PHASE_0_EXAMPLES.md           # Phase 0 guide
│   └── ARCHITECTURE.md               # System architecture
├── .github/workflows/
│   └── ci.yml                        # Multi-platform CI
├── .clang-format                     # C++ code style
├── .clang-tidy                       # C++ linting
├── CMakeLists.txt                    # Root build file
├── pixi.toml                         # Environment management
├── sonar-project.properties          # SonarCloud config
├── README.md                         # Project overview
└── CLAUDE.md                         # This file
```

## Technology Stack

- **Build System:** CMake 3.14+ with `CMAKE_EXPORT_COMPILE_COMMANDS=ON`
- **Testing:** GoogleTest 1.14+ (C++), Jest (JavaScript)
- **Coverage:** gcov/gcovr (C++), c8 (JavaScript)
- **Environment:** Pixi (replaces Docker/conda)
- **CI/CD:** GitHub Actions
- **Quality:** SonarCloud with CFamily sensor
- **Hardware:** PlatformIO for Arduino uploads

## Development Standards

### Testing Requirements
- **Core logic:** 80%+ coverage minimum
- **Hardware wrappers:** Integration tests with mocks
- **UI/Preview tools:** Manual testing acceptable

### Hardware Abstraction
- Business logic NEVER calls hardware directly
- Interface classes for all hardware (IServoController, ISensor)
- Mock implementations for testing
- .ino files are thin wrappers only (<50 lines)

### Code Quality
- No SonarCloud bugs or vulnerabilities
- Fix code smells when reasonable
- Use clang-format for C++ (Google style)
- Follow modern C++ patterns (RAII, smart pointers, constexpr)

### Documentation Hygiene
- Keep minimal and focused (<10 markdown files ideal)
- Merge aggressively when docs overlap
- Archive or delete outdated documentation
- Each piece of info lives in ONE place
- Update docs as part of implementation, not after

## Pixi Environment

All projects MUST have standardized pixi tasks:

### Required Tasks
- `pixi run test` - Run all unit tests
- `pixi run coverage` - Generate code coverage report
- `pixi run view-coverage` - Open coverage in browser

### Example pixi.toml (trivial_math)
```toml
[tasks]
test-trivial = "cmake -S. -B build/test -DBUILD_TESTS=ON && cmake --build build/test && ctest --test-dir build/test --output-on-failure"
coverage-trivial = "cmake -S. -B build/coverage -DBUILD_TESTS=ON -DENABLE_COVERAGE=ON && cmake --build build/coverage && cd build/coverage && ctest && gcovr -r ../.. --sonarqube ../../coverage.xml --print-summary"
view-coverage-trivial = "xdg-open build/coverage/coverage-trivial/index.html"
```

## SonarCloud Integration

### Configuration
- Project key: `griswaldbrooks_halloween2`
- Organization: `griswaldbrooks`
- Multi-language: C++, JavaScript (Python later)
- Coverage format: gcovr SonarQube XML (C++), LCOV (JavaScript)

### Quality Gates
After pushing to GitHub:
1. Wait for SonarCloud analysis to complete
2. Review dashboard for issues
3. Fix bugs and vulnerabilities immediately
4. Address code smells if reasonable
5. Verify coverage reported correctly
6. Push fixes and re-verify

### Verification Tool
Use halloween repo tool for ground truth:
```bash
python /home/griswald/personal/halloween/tools/sonarcloud_verify.py \
    --project griswaldbrooks_halloween2
```

## Parent Project Reference

This is a rewrite of: `/home/griswald/personal/halloween`

**Lessons Learned from Halloween 1.0:**
1. compile_commands.json is REQUIRED for CFamily sensor
2. Code duplication breaks coverage attribution
3. Path resolution is fragile - test early
4. Tool verification is essential - don't trust UI alone
5. SonarCloud "fixes" can break functionality - always test after
6. Hardware abstraction is critical for testability

**Key Successes to Replicate:**
- Spider crawl projection: 24% → 94% coverage via incremental extraction
- Hatching egg: 85.9% C++ coverage with hardware abstraction
- Test-driven approach prevented regressions

## Communication Standards

When reporting results:
- ✅ **Do** verify locally first (run tests, check coverage)
- ✅ **Do** use tools to verify external systems (SonarCloud API)
- ✅ **Do** show actual output, not assumptions
- ✅ **Do** be specific about what was tested
- ❌ **Don't** claim external services work without verification
- ❌ **Don't** rely on dashboard screenshots alone
- ⚠️ **Be honest** about what cannot be verified (hardware behavior)

## Current Focus

**Phase 0.1: trivial_math (Pure C++ Library)**
- Implement header-only math utilities
- Write GoogleTest tests achieving 100% coverage
- Configure CMake with coverage flags
- Generate gcovr reports (XML for SonarCloud, HTML for local)
- Verify coverage reports correctly
- Document the pattern for future examples

**Next:** Phase 0.2 (blink_led), Phase 0.3 (web_trigger)

## Implementation Plan Reference

See `docs/CLEAN_SLATE_IMPLEMENTATION_PLAN.md` for:
- Complete phase breakdown (Phases 0-5)
- Detailed timelines
- Success criteria per phase
- Migration strategy from halloween 1.0
- Risk mitigation approaches

## Timeline

- **Phase 0:** 2-3 weeks (Architecture validation)
- **Phase 1:** 4-6 weeks (Core libraries)
- **Phase 2:** 6-8 weeks (Hatching egg migration)
- **Phase 3:** 4-6 weeks (Spider projection migration)
- **Phase 4:** 6-8 weeks (Window spider + Body)
- **Phase 5:** 2-3 weeks (Polish and docs)

**Total:** 24-34 weeks (6-8.5 months)

## Success Metrics

At completion of Phase 0:
- 3/3 examples building
- 100% tests passing
- 80%+ coverage reported to SonarCloud for each
- CI/CD green for all examples
- Documentation complete and validated
- Zero SonarCloud bugs/vulnerabilities

---

**Created:** 2025-11-12
**Purpose:** Production-ready Halloween animatronics with validated architecture
**Approach:** Phase 0 validation first, then incremental legacy migration
**Parent Project:** [halloween](https://github.com/griswaldbrooks/halloween)
