# SonarCloud C++ Minimal - Development Tools

Comprehensive tooling for checking CI/CD and SonarCloud status.

## Overview

This toolset provides quick, reliable verification of project health:

1. **sonarcloud_check.py** - Check SonarCloud analysis status
2. **ci_check.py** - Check GitHub Actions workflow status
3. **status.py** - Combined status check (both CI and SonarCloud)

## Requirements

```bash
# Python 3 (already installed on most systems)
python3 --version

# Required Python packages
pip install requests  # For SonarCloud API calls

# GitHub CLI (for CI checks)
# Install from: https://cli.github.com/
gh --version
```

## Quick Start

```bash
# Check everything at once
python tools/status.py

# Or use pixi
pixi run status
```

## Tool Documentation

### 1. sonarcloud_check.py

**Purpose:** Quick SonarCloud project status verification

**Usage:**
```bash
# Check project status
python tools/sonarcloud_check.py --project griswaldbrooks_sonarcloud-cpp-minimal

# Check specific component
python tools/sonarcloud_check.py --project griswaldbrooks_sonarcloud-cpp-minimal --component src
```

**Output includes:**
- Last analysis timestamp
- Quality gate status (OK/ERROR)
- Coverage percentage
- Bug count
- Vulnerability count
- Code smell count
- Per-file coverage breakdown

**Example output:**
```
======================================================================
SonarCloud Status Check
======================================================================
Project: griswaldbrooks_sonarcloud-cpp-minimal
Last Analysis: 2025-11-12 18:30:45 UTC

Quality Gate: ✅ OK

Project Metrics:
----------------------------------------------------------------------
  Coverage: ✅ 100.0%
  Bugs: ✅ 0
  Vulnerabilities: ✅ 0
  Code Smells: 0
  Security Hotspots: 0
  Lines of Code: 10

File Coverage:
----------------------------------------------------------------------
  ✅ src/math.cpp: 100.0% (4 lines, 0 uncovered)
  ✅ src/math.h: 100.0% (2 lines, 0 uncovered)

Total Files: 2
With Coverage: 2
Without Coverage: 0

======================================================================
```

**Exit codes:**
- `0` - Success, quality gate passed
- `1` - Error or quality gate failed

### 2. ci_check.py

**Purpose:** Check GitHub Actions workflow run status

**Usage:**
```bash
# Check recent runs (default: 5)
python tools/ci_check.py

# Check more runs
python tools/ci_check.py --limit 10

# Check specific workflow
python tools/ci_check.py --workflow coverage.yml
```

**Output includes:**
- Repository information
- Recent workflow runs with status
- Success/failure summary

**Example output:**
```
======================================================================
GitHub Actions CI Status
======================================================================
Repository: griswaldbrooks/sonarcloud-cpp-minimal

Recent Runs (showing 5):
----------------------------------------------------------------------
✅ SUCCESS - Coverage Analysis - Add initial coverage setup (#12)
   Branch: main | Time: 2025-11-12 18:30:00
✅ SUCCESS - Coverage Analysis - Update CMake config (#11)
   Branch: main | Time: 2025-11-12 17:45:00
❌ FAILED - Coverage Analysis - Fix gcovr paths (#10)
   Branch: main | Time: 2025-11-12 17:20:00
✅ SUCCESS - Coverage Analysis - Initial commit (#9)
   Branch: main | Time: 2025-11-12 16:50:00
🔄 IN PROGRESS - Coverage Analysis - Update tests (#8)
   Branch: feature/tests | Time: 2025-11-12 16:30:00

Summary:
----------------------------------------------------------------------
  Successful: 3
  Failed: 1
  Other: 1

======================================================================
```

**Exit codes:**
- `0` - Success, most recent run passed
- `1` - Most recent run failed or error

**Requirements:**
- GitHub CLI (`gh`) must be installed and authenticated
- Must be run from within git repository with GitHub remote

### 3. status.py

**Purpose:** Combined CI and SonarCloud status check

**Usage:**
```bash
# Check everything (recommended)
python tools/status.py

# Specify project explicitly
python tools/status.py --project griswaldbrooks_sonarcloud-cpp-minimal

# Show more CI runs
python tools/status.py --ci-limit 10

# Skip CI check
python tools/status.py --skip-ci

# Skip SonarCloud check
python tools/status.py --skip-sonarcloud
```

**Output includes:**
- Full CI status report
- Full SonarCloud status report
- Overall pass/fail summary

**Exit codes:**
- `0` - All checks passed
- `1` - One or more checks failed

## Pixi Integration

Add to your `pixi.toml`:

```toml
[tasks]
# Individual checks
check-ci = "python tools/ci_check.py"
check-sonarcloud = "python tools/sonarcloud_check.py --project griswaldbrooks_sonarcloud-cpp-minimal"

# Combined status
status = "python tools/status.py"

# Verbose checks
status-verbose = "python tools/status.py --ci-limit 10"
```

Then use:

```bash
pixi run status              # Quick status check
pixi run check-ci            # Just CI
pixi run check-sonarcloud    # Just SonarCloud
pixi run status-verbose      # More detail
```

## Common Use Cases

### 1. Pre-Push Verification

Before pushing code, verify everything is healthy:

```bash
# Run tests locally
pixi run test

# Check if previous push succeeded
python tools/status.py
```

### 2. Post-Push Verification

After pushing code and waiting for CI:

```bash
# Wait for CI to complete (check GitHub Actions)
# Then verify results
python tools/status.py

# Or check just SonarCloud
python tools/sonarcloud_check.py --project griswaldbrooks_sonarcloud-cpp-minimal
```

### 3. Coverage Verification

After implementing coverage and pushing:

```bash
# Check if SonarCloud has coverage data
python tools/sonarcloud_check.py --project griswaldbrooks_sonarcloud-cpp-minimal

# Look for "Coverage: ✅ XX.X%"
# If "Coverage: ❌ No data", integration failed
```

### 4. CI Debugging

When CI fails:

```bash
# Check recent runs
python tools/ci_check.py --limit 10

# Identify which workflow failed
# Check logs in GitHub Actions
```

### 5. Quality Gate Monitoring

Monitor if code meets quality standards:

```bash
# Check quality gate status
python tools/sonarcloud_check.py --project griswaldbrooks_sonarcloud-cpp-minimal

# Look for "Quality Gate: ✅ OK"
# If "Quality Gate: ❌ ERROR", review issues in SonarCloud
```

## Integration with CI/CD

Add to `.github/workflows/coverage.yml`:

```yaml
- name: Wait for SonarCloud Analysis
  run: |
    sleep 30  # Give SonarCloud time to process

- name: Verify SonarCloud Status
  run: |
    pip install requests
    python tools/sonarcloud_check.py \
      --project griswaldbrooks_sonarcloud-cpp-minimal

- name: Check Coverage Threshold
  run: |
    python tools/sonarcloud_check.py \
      --project griswaldbrooks_sonarcloud-cpp-minimal | \
      grep "Coverage:" | grep -E "[8-9][0-9]|100" || \
      (echo "Coverage below 80%" && exit 1)
```

## Comparison with halloween/tools/sonarcloud_verify.py

This project's tools are **simplified** versions optimized for quick checks:

### sonarcloud_verify.py (halloween repo)
- Comprehensive diagnostic tool
- C++ coverage deep-dive features
- lcov.info comparison
- Detailed file-by-file analysis
- Full test coverage
- ~600 lines

### sonarcloud_check.py (this repo)
- Quick status checks
- Essential metrics only
- Simplified output
- Fast execution
- ~250 lines

**When to use which:**

Use **sonarcloud_check.py** when:
- Quick status check needed
- Pre-commit verification
- CI/CD integration
- Just want pass/fail

Use **sonarcloud_verify.py** when:
- Debugging coverage issues
- Comparing local vs SonarCloud
- Deep C++ coverage investigation
- Need comprehensive reports

## Troubleshooting

### "Error: Failed to communicate with SonarCloud API"

**Causes:**
- Internet connection down
- SonarCloud API unavailable
- Invalid project key

**Solutions:**
```bash
# Check internet
ping sonarcloud.io

# Verify project key
echo "griswaldbrooks_sonarcloud-cpp-minimal"

# Check SonarCloud status
open https://status.sonarcloud.io/
```

### "Error: gh CLI not found"

**Cause:** GitHub CLI not installed or not in PATH

**Solution:**
```bash
# Install gh CLI
# Linux/macOS: https://cli.github.com/
# Or use package manager:
# Ubuntu: sudo apt install gh
# macOS: brew install gh

# Authenticate
gh auth login
```

### "Error: Unable to detect GitHub repository"

**Cause:** Not in a git repository or no GitHub remote

**Solution:**
```bash
# Verify git repo
git remote -v

# Should show GitHub URL
# If not, add remote:
git remote add origin https://github.com/griswaldbrooks/sonarcloud-cpp-minimal.git
```

### "Coverage: ❌ No data" in SonarCloud

**Causes:**
- CI hasn't run yet
- Coverage upload failed
- SonarCloud still processing

**Solutions:**
```bash
# 1. Check CI completed
python tools/ci_check.py

# 2. Wait for SonarCloud processing (can take 5-10 minutes)
sleep 300

# 3. Check again
python tools/sonarcloud_check.py --project griswaldbrooks_sonarcloud-cpp-minimal

# 4. If still no data, check CI logs for upload errors
```

## Tool Philosophy

These tools embody the project's "verify with tools, not assumptions" philosophy:

**Principles:**
1. **API-based verification** - Query actual state, don't guess from UI
2. **Fast feedback** - Quick checks for rapid iteration
3. **Actionable output** - Clear pass/fail, no ambiguity
4. **Scriptable** - Exit codes for automation
5. **Documented** - Clear usage and examples

**Workflow:**
```
Write code → Test locally → Push → tools/status.py → Fix if needed → Repeat
```

## Future Enhancements

Potential improvements:

- **JSON output mode** for easier parsing
- **Coverage trend tracking** (compare across commits)
- **Slack/Discord notifications** on failures
- **Automatic issue creation** on quality gate failures
- **Dashboard generation** (HTML report)
- **Multi-project support** (for monorepos)

## Related Documentation

- [CLAUDE.md](/home/griswald/personal/sonarcloud-cpp-minimal/CLAUDE.md) - Project overview and standards
- [SETUP.md](/home/griswald/personal/sonarcloud-cpp-minimal/SETUP.md) - SonarCloud setup instructions
- [PROJECT_SUMMARY.md](/home/griswald/personal/sonarcloud-cpp-minimal/PROJECT_SUMMARY.md) - Investigation framework
- [halloween/tools/README.md](/home/griswald/personal/halloween/tools/README.md) - Full-featured verification tools

---

**Created:** 2025-11-12
**Purpose:** Quick, reliable project health verification
**Philosophy:** Build tools to know the truth, don't rely on assumptions
