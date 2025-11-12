# SonarCloud C++ Coverage Minimal Reproduction

Minimal C++ project to isolate and understand SonarCloud C++ coverage issues.

## Purpose

This project is designed to:
- Test SonarCloud C++ coverage in the simplest possible setup
- Isolate whether the issue is specific to monorepos or fundamental to our approach
- Provide a minimal reproduction case for debugging or support requests

## Structure

```
src/
  math.h          # Simple header with function declarations
  math.cpp        # Simple implementation (2 functions)
test/
  test_math.cpp   # GoogleTest tests achieving 100% coverage
```

## Build and Test

```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build

# Run tests
cd build && ctest --output-on-failure

# Generate coverage
gcovr -r . --sonarqube coverage.xml --exclude test/ --print-summary
```

## Expected Results

**Local Coverage:** 100% (all lines in math.cpp covered)

**SonarCloud Coverage:**
- If this works: We know the issue is with our monorepo setup
- If this fails: We know something fundamental is wrong with our approach

## Key Configuration

- **CMake:** Exports compile_commands.json, enables --coverage flag
- **gcovr:** Generates SonarQube XML format
- **SonarCloud:** Uses cfamily.compile-commands and coverageReportPaths
- **NO projectBaseDir:** Keeping it as simple as possible

## Differences from cmake_prototype

- Single project (not monorepo)
- No projectBaseDir complexity
- Minimal code (10 lines vs 100s)
- Direct paths (no subdirectories)

## Investigation Results

[To be filled in after CI/CD run and verification]
