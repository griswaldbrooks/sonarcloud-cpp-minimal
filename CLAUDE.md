# SonarCloud C++ Coverage Minimal Reproduction Project

## Project Overview

This is a minimal C++ reproduction project for investigating SonarCloud coverage display issues. It serves as an isolated test case to determine whether coverage integration problems are monorepo-related or fundamental to the SonarCloud configuration.

**Status:** Ready for SonarCloud configuration and testing

## Purpose

Determine root cause of SonarCloud C++ coverage not displaying by testing in a minimal, single-project repository:
- **If coverage works here:** Issue is monorepo-related (multi-project complexity)
- **If coverage fails here:** Issue is fundamental (configuration or SonarCloud limitation)

## Background

This project was created after 3 failed attempts to fix C++ coverage in the halloween/cmake_prototype project:

1. **Attempt 1:** sonar.cfamily.gcov.pathPrefix (failed)
2. **Attempt 2:** sonar.sources path adjustment (failed)
3. **Attempt 3:** compile_commands.json path workaround (failed)

All attempts showed same symptom: local coverage = 100%, SonarCloud coverage = 0%

**Hypothesis:** The halloween repo's monorepo structure (multiple projects in subdirectories) may be causing path resolution issues that prevent SonarCloud from correctly attributing coverage data.

## Project Structure

```
sonarcloud-cpp-minimal/
├── src/
│   ├── math_utils.h          # Header with inline functions
│   └── math_utils.cpp         # Implementation file
├── test/
│   └── test_math_utils.cpp    # GoogleTest unit tests
├── CMakeLists.txt             # Build configuration with coverage
├── sonar-project.properties   # SonarCloud configuration
├── .github/workflows/         # CI with coverage generation
├── SETUP.md                   # Step-by-step setup instructions
├── PROJECT_SUMMARY.md         # Investigation framework
└── CLAUDE.md                  # This file
```

## Technology Stack

- **Build System:** CMake 3.14+ with `CMAKE_EXPORT_COMPILE_COMMANDS=ON`
- **Testing:** GoogleTest (gtest 1.12+)
- **Coverage Generation:** gcovr with SonarQube XML output format
- **CI/CD:** GitHub Actions
- **Analysis:** SonarCloud with CFamily sensor
- **Verification:** API-based tool from halloween repo

## Key Configuration

**CMakeLists.txt features:**
- `CMAKE_EXPORT_COMPILE_COMMANDS=ON` - Required for SonarCloud CFamily sensor
- `-fprofile-arcs -ftest-coverage` - GCC coverage flags
- gcovr generates both HTML (local) and XML (SonarCloud) formats

**sonar-project.properties:**
- `sonar.projectKey=griswaldbrooks_sonarcloud-cpp-minimal`
- `sonar.organization=griswaldbrooks`
- `sonar.sources=src`
- `sonar.tests=test`
- `sonar.coverageReportPaths=coverage.xml`

**GitHub Actions workflow:**
- Builds with coverage flags
- Runs tests
- Generates gcovr XML report
- Uploads to SonarCloud

## Success Criteria

### If Coverage Works (Displays in SonarCloud)
- **Conclusion:** Issue is monorepo-related
- **Next Step:** Compare this config with halloween/cmake_prototype line-by-line
- **Look for:** Path differences, sonar.projectBaseDir usage, multi-module setup
- **Action:** Apply working config to halloween repo

### If Coverage Fails (0% in SonarCloud)
- **Conclusion:** Fundamental issue with gcovr + SonarCloud C++
- **Next Step:** File SonarCloud support ticket with this minimal reproduction
- **Evidence:** Simple project, correct formats, still doesn't work
- **Action:** Wait for SonarCloud guidance or explore alternative approaches

## Next Steps (For Next Agent)

1. **Verify SONAR_TOKEN is configured** (user must add as GitHub secret)
2. **Trigger CI/CD:** Commit and push to start analysis
3. **Wait for completion:** GitHub Actions + SonarCloud analysis
4. **Verify with tool:**
   ```bash
   python /home/griswald/personal/halloween/tools/sonarcloud_verify.py \
       --project griswaldbrooks_sonarcloud-cpp-minimal
   ```
5. **Analyze results** using framework in PROJECT_SUMMARY.md
6. **Document findings** and determine next action

## Tools Available

### sonarcloud_verify.py
Located in halloween repo: `/home/griswald/personal/halloween/tools/sonarcloud_verify.py`

**Usage:**
```bash
# Check coverage state
python /home/griswald/personal/halloween/tools/sonarcloud_verify.py \
    --project griswaldbrooks_sonarcloud-cpp-minimal

# Compare with local coverage
python /home/griswald/personal/halloween/tools/sonarcloud_verify.py \
    --project griswaldbrooks_sonarcloud-cpp-minimal \
    --compare-local coverage.xml
```

**What it reveals:**
- Actual coverage percentages per file (not just dashboard view)
- Which files SonarCloud has coverage data for
- Path mismatches between local and SonarCloud
- Comparison with expected local coverage

See `/home/griswald/personal/halloween/tools/README.md` for full documentation.

## Reference Documentation

**In this repo:**
- `README.md` - Project description and build instructions
- `SETUP.md` - Step-by-step SonarCloud setup
- `PROJECT_SUMMARY.md` - Analysis framework and decision tree
- `NEXT_AGENT_QUICKSTART.md` - Quick start for next session

**In halloween repo:**
- `/home/griswald/personal/halloween/CLAUDE.md` - Full project conventions
- `/home/griswald/personal/halloween/cmake_prototype/` - Original prototype
- `/home/griswald/personal/halloween/tools/` - Verification tools
- `/home/griswald/personal/halloween/SESSION_2025-11-12_CMAKE_PROTOTYPE.md` - Full investigation history

## Key Insights from Halloween Investigation

1. **compile_commands.json is required** - CFamily sensor won't analyze without it
2. **gcovr SonarQube XML is correct format** - Not .gcov or LCOV
3. **Code duplication breaks attribution** - Single source of truth is critical
4. **Tool verification is essential** - Don't trust UI alone, use API tools
5. **Path resolution is fragile** - Small path mismatches cause silent failures

## Expected Timeline

- **User action:** Add SONAR_TOKEN secret (2 minutes)
- **CI/CD run:** 3-5 minutes
- **SonarCloud analysis:** 2-3 minutes
- **Tool verification:** 30 seconds
- **Analysis:** 10-15 minutes
- **Total:** ~20 minutes to definitive answer

## Communication with User

When reporting results:
- ✅ **Do** verify with tool before claiming success/failure
- ✅ **Do** show actual API output, not assumptions
- ✅ **Do** be specific about what was tested
- ❌ **Don't** rely on dashboard screenshots alone
- ❌ **Don't** claim things work without API verification
- ❌ **Don't** guess about SonarCloud state

## This is a Focused Investigation

This project exists for ONE purpose: answer the monorepo vs fundamental question.

**Keep it simple:**
- No additional features
- No complex test scenarios
- No optimization attempts
- Just: does coverage display or not?

Once we have the answer, this repo may be archived or deleted. Its value is in providing clarity, not in long-term maintenance.

---

**Created:** 2025-11-12
**Purpose:** Isolate SonarCloud C++ coverage issue
**Parent Project:** `/home/griswald/personal/halloween`
**Related:** halloween/cmake_prototype (0% coverage in SonarCloud despite 100% local)
