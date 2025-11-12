# Next Agent Quick Start

## What This Project Is

Minimal C++ reproduction project to isolate SonarCloud coverage display issue. Simple codebase (2 source files, 1 test file) to determine if the problem is monorepo-related or fundamental.

## Current State

**What's Done:**
- ✅ Project created with CMake, GoogleTest, gcovr
- ✅ Code committed and pushed to GitHub
- ✅ GitHub Actions workflow configured
- ✅ SonarCloud project created
- ⚠️ **WAITING:** User needs to add SONAR_TOKEN to GitHub secrets

**What's Not Done:**
- CI/CD has not run yet (needs SONAR_TOKEN)
- SonarCloud has not analyzed the code yet
- Coverage state unknown

## What Happened Before

**Context from halloween repo:**

The halloween/cmake_prototype project has a coverage display issue:
- **Local coverage:** 100% (21 GoogleTest tests, gcovr report shows all lines covered)
- **SonarCloud coverage:** 0% (dashboard shows no coverage data)

**Three failed fix attempts:**
1. Added `sonar.cfamily.gcov.pathPrefix=cmake_prototype` - Failed
2. Adjusted `sonar.sources=cmake_prototype` - Failed
3. Copied compile_commands.json to repo root - Failed

**Result:** Same symptom every time - local coverage perfect, SonarCloud shows 0%

**Hypothesis:** The halloween repo's monorepo structure (multiple projects in subdirectories) may be causing path resolution issues. This minimal single-project repo will test that hypothesis.

## Your Task

### Step 1: Verify SONAR_TOKEN (User Action Required)

**IMPORTANT:** User must add SONAR_TOKEN to GitHub repository secrets before you can proceed.

**How to verify it's added:**
```bash
# This won't show the token value (that's secret), but you can check if workflow runs
cd /home/griswald/personal/sonarcloud-cpp-minimal
git log -1 --oneline  # Should show recent commits
```

**Wait for user confirmation:** "I've added the SONAR_TOKEN secret"

### Step 2: Trigger CI/CD

Once user confirms token is added:
```bash
cd /home/griswald/personal/sonarcloud-cpp-minimal
git commit --allow-empty -m "Trigger CI for SonarCloud analysis"
git push
```

### Step 3: Monitor CI/CD

**Check GitHub Actions:**
```bash
# Get repository URL
cd /home/griswald/personal/sonarcloud-cpp-minimal
git remote get-url origin
```

**Expected workflow steps:**
1. Checkout code
2. Install dependencies (cmake, g++, gcovr)
3. Build with coverage flags
4. Run tests (should pass)
5. Generate gcovr XML report
6. SonarCloud Scan action
7. Upload coverage to SonarCloud

**Typical runtime:** 3-5 minutes for CI, 2-3 minutes for SonarCloud analysis

### Step 4: Verify with Tool

**CRITICAL:** Do NOT trust the dashboard UI alone. Use the API verification tool:

```bash
python /home/griswald/personal/halloween/tools/sonarcloud_verify.py \
    --project griswaldbrooks_sonarcloud-cpp-minimal
```

**Expected output if working:**
```
Project: griswaldbrooks_sonarcloud-cpp-minimal
Component: sonarcloud-cpp-minimal:src/math_utils.cpp
Coverage: 100.0%
Lines: 4/4
```

**Expected output if failing:**
```
Project: griswaldbrooks_sonarcloud-cpp-minimal
Component: sonarcloud-cpp-minimal:src/math_utils.cpp
Coverage: 0.0% or "No coverage data"
```

### Step 5: Analyze Results

**If Coverage Works (Shows >0% in tool):**
- ✅ **Conclusion:** Issue is monorepo-related
- **Next action:** Compare configs between this repo and halloween/cmake_prototype
- **Look for:**
  - sonar.projectBaseDir differences
  - Path structure (root vs subdirectory)
  - Multi-module configuration
- **Deliverable:** Document the specific configuration difference that makes it work

**If Coverage Fails (Shows 0% in tool):**
- ⚠️ **Conclusion:** Fundamental issue with gcovr + SonarCloud C++
- **Next action:** Gather evidence for SonarCloud support ticket
- **Evidence needed:**
  - Tool output showing 0% coverage
  - Link to CI logs showing successful gcovr generation
  - coverage.xml file contents
  - SonarCloud analysis logs
- **Deliverable:** Formatted support ticket with minimal reproduction

### Step 6: Document Findings

Create `RESULTS.md` in this repo with:
- Tool verification output (copy/paste exact output)
- CI/CD logs (relevant sections)
- Decision: monorepo issue or fundamental issue
- Next recommended action
- Comparison with halloween repo (if relevant)

## Key Files

- **SETUP.md** - Step-by-step setup instructions (reference if needed)
- **PROJECT_SUMMARY.md** - Analysis framework and decision tree
- **CLAUDE.md** - Full project context and conventions
- **sonar-project.properties** - SonarCloud configuration

## Tools Available

### sonarcloud_verify.py

**Location:** `/home/griswald/personal/halloween/tools/sonarcloud_verify.py`

**Documentation:** `/home/griswald/personal/halloween/tools/README.md`

**Usage:**
```bash
# Basic check
python /home/griswald/personal/halloween/tools/sonarcloud_verify.py \
    --project griswaldbrooks_sonarcloud-cpp-minimal

# With local comparison
python /home/griswald/personal/halloween/tools/sonarcloud_verify.py \
    --project griswaldbrooks_sonarcloud-cpp-minimal \
    --compare-local coverage.xml
```

**Why use it:**
- Gets ground truth from SonarCloud API
- Shows actual coverage percentages
- Reveals path mismatches
- More reliable than dashboard UI

## Expected Timeline

- User adds SONAR_TOKEN: 2 minutes
- Trigger CI/CD: 30 seconds
- CI/CD completes: 3-5 minutes
- SonarCloud analysis: 2-3 minutes
- Tool verification: 30 seconds
- Results analysis: 10-15 minutes
- **Total: ~20 minutes to answer**

## Red Flags

**If you see these, investigate:**
- ❌ CI fails to build: Check CMakeLists.txt, dependencies
- ❌ Tests fail: Check test code, verify logic is correct
- ❌ gcovr fails: Check coverage flags, file paths
- ❌ SonarCloud scan errors: Check token, project key, organization
- ❌ Tool shows "Project not found": Check project key spelling

## Success Criteria

**You've completed the task when you can answer:**

1. **Does coverage display in SonarCloud?** (Yes/No, verified with tool)
2. **What's the actual coverage percentage?** (From tool, not dashboard)
3. **Is this a monorepo issue or fundamental issue?** (Clear conclusion)
4. **What's the recommended next action?** (Specific, actionable)

## Communication Guidelines

**When reporting to user:**
- ✅ Show actual tool output (copy/paste)
- ✅ Link to CI logs if relevant
- ✅ Be specific about what was verified
- ✅ State confidence level in conclusion
- ❌ Don't claim success without tool verification
- ❌ Don't rely on dashboard screenshots alone
- ❌ Don't guess about SonarCloud internal state

## If You Get Stuck

**Common issues:**

1. **Tool shows "No coverage data"**
   - Check CI logs: did gcovr actually run?
   - Check coverage.xml: is it generated correctly?
   - Check SonarCloud logs: did it parse the file?

2. **CI fails mysteriously**
   - Check GitHub Actions tab for detailed logs
   - Look for authentication errors (SONAR_TOKEN issue)
   - Verify dependencies installed correctly

3. **Can't run tool**
   - Check Python version: `python --version` (should be 3.x)
   - Check requests library: `python -c "import requests"`
   - Check tool exists: `ls /home/griswald/personal/halloween/tools/sonarcloud_verify.py`

4. **Results are ambiguous**
   - Re-run tool to confirm
   - Check SonarCloud dashboard manually as secondary verification
   - Review CI logs for any warnings or errors
   - Compare with halloween repo setup side-by-side

## Background Reading

If you need more context:
- `/home/griswald/personal/halloween/CLAUDE.md` - Full halloween project conventions
- `/home/griswald/personal/halloween/cmake_prototype/README.md` - Original prototype docs
- `/home/griswald/personal/halloween/SESSION_2025-11-12_CMAKE_PROTOTYPE.md` - Investigation history
- `/home/griswald/personal/halloween/tools/README.md` - Tool documentation

## Remember

This is a **focused investigation** to answer ONE question: monorepo issue or fundamental issue?

Don't get distracted by:
- Code quality improvements
- Additional test cases
- Performance optimization
- Documentation polish

**Just answer the question, document the findings, recommend next action.**

---

**Good luck! The answer is ~20 minutes away.**
