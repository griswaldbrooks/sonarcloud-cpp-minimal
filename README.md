# Halloween 2.0 - Modern C++ Animatronics

A clean-slate rewrite of the Halloween haunted house animatronics system with modern C++ architecture, comprehensive testing, and multi-platform support.

## Project Philosophy

**Phase 0 First:** Before migrating any legacy code, we validate our architecture with trivial examples across all target configurations. This ensures:
- Build systems work correctly
- Tests run reliably
- Coverage reports accurately
- Multi-language integration functions properly

## Current Phase: Phase 0 - Architecture Validation

Implementing three example configurations to validate the complete toolchain:

1. **Pure C++ Library** (`lib/examples/trivial_math/`)
   - Platform-agnostic logic library
   - GoogleTest tests
   - gcov/gcovr coverage reporting
   - Target: 100% coverage

2. **C++ + Arduino (.ino)** (`projects/examples/blink_led/`)
   - Minimal .ino wrapper (<50 lines)
   - C++ logic library
   - Desktop tests for logic
   - Arduino compilation for hardware
   - Coverage for C++ components

3. **JavaScript + C++ + Arduino** (`projects/examples/web_trigger/`)
   - Node.js/Express server
   - C++ Arduino component
   - Serial/WebSocket communication
   - Jest tests (JavaScript)
   - GoogleTest tests (C++)
   - Dual-language coverage reporting

## Phase 0 Success Criteria

- All 3 examples build successfully
- All tests pass (100%)
- Coverage reports to SonarCloud (80%+ for all)
- CI/CD runs all examples automatically
- Documentation demonstrates each pattern clearly

**Timeline:** 2-3 weeks before Phase 1 (legacy migration)

## Quick Start

```bash
# Install dependencies via Pixi
pixi install

# Test trivial_math example (Phase 0.1)
pixi run test-trivial

# Generate coverage report
pixi run coverage-trivial

# View coverage locally
xdg-open build/coverage/coverage-trivial/index.html
```

## Repository Structure

```
halloween2/
├── lib/
│   ├── animatronics_core/       # Future: Core libraries
│   └── examples/                # Phase 0: Architecture validation
│       └── trivial_math/        # Pure C++ example
├── projects/
│   └── examples/                # Phase 0: Project patterns
│       ├── blink_led/           # C++ + .ino example
│       └── web_trigger/         # JS + C++ + .ino example
├── toolchains/                  # CMake toolchain files
├── docs/                        # Project documentation
├── pixi.toml                    # Environment management
└── sonar-project.properties     # SonarCloud config
```

## Technology Stack

- **Build System:** CMake 3.14+ with compile_commands.json
- **Testing:** GoogleTest (C++), Jest (JavaScript)
- **Coverage:** gcov/gcovr (C++), c8 (JavaScript)
- **Environment:** Pixi (replaces Docker/conda)
- **CI/CD:** GitHub Actions
- **Quality:** SonarCloud analysis
- **Hardware:** Arduino (Leonardo, Nano, Beetle), NodeMCU, RPi Pico

## Documentation

- [docs/CLEAN_SLATE_IMPLEMENTATION_PLAN.md](docs/CLEAN_SLATE_IMPLEMENTATION_PLAN.md) - Full implementation roadmap
- [docs/PHASE_0_EXAMPLES.md](docs/PHASE_0_EXAMPLES.md) - Phase 0 validation guide
- [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) - System architecture
- [CLAUDE.md](CLAUDE.md) - AI agent instructions and context

## Parent Project

This is a clean-slate rewrite of the [halloween](https://github.com/griswaldbrooks/halloween) project. The original repo contains:
- Legacy implementations (hatching_egg, spider_crawl_projection, etc.)
- Investigation history and lessons learned
- Testing tools and utilities

## Build Requirements

- CMake 3.14+
- GCC/Clang with C++17 support
- GoogleTest 1.14+
- gcovr 7.0+
- Node.js 18+ (for JavaScript examples)
- PlatformIO (for Arduino uploads)

All dependencies managed via Pixi - see `pixi.toml` for details.

## Development Workflow

1. **Start with Phase 0:** Validate architecture with trivial examples
2. **Test-driven:** Write tests before implementation (80%+ coverage goal)
3. **Hardware abstraction:** Business logic never calls hardware directly
4. **Incremental commits:** Small, focused changes with descriptive messages
5. **SonarCloud verification:** Fix issues before considering work complete

## Quality Standards

- **Test Coverage:** 80%+ across all languages
- **SonarCloud:** No bugs or vulnerabilities, minimal code smells
- **Hardware Independence:** Core logic testable without physical devices
- **Documentation:** Keep minimal, current, and actionable
- **Code Style:** clang-format for C++, prettier for JavaScript

## Current Status

**Phase 0.1: trivial_math (Pure C++)**
- Status: Complete
- Implementation: ✅ Complete
- Tests: ✅ Complete (3 tests)
- Coverage: ✅ 100%
- SonarCloud: Pending

**Phase 0.2: blink_led (C++ + Arduino)**
- Status: Complete
- Implementation: ✅ Complete
- .ino wrapper: ✅ 32 lines (15 LOC)
- Tests: ✅ Complete (11 tests)
- Coverage: ✅ 100%
- Arduino compilation: Not tested
- SonarCloud: Pending

**Phase 0.3: web_trigger (JavaScript + C++ + Arduino)**
- Status: Not started

See [docs/PHASE_0_EXAMPLES.md](docs/PHASE_0_EXAMPLES.md) for detailed progress.

---

**Created:** 2025-11-12
**Purpose:** Production-ready Halloween animatronics with modern architecture
**Approach:** Validate architecture first, then migrate legacy code incrementally
