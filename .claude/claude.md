# Claude Code Agent Instructions - Halloween Animatronics Repository

[![Unit Tests](https://github.com/griswaldbrooks/halloween/actions/workflows/test.yml/badge.svg)](https://github.com/griswaldbrooks/halloween/actions/workflows/test.yml)

## Repository Overview

**Multi-year Halloween haunted house animatronics** - Production-ready, reusable components.

**Status:** ✅ **All systems operational, CI passing, ready for next year**

This repository contains 4 complete animatronic projects that can be reused year after year:
- **hatching_egg** - Spider egg with 4 servos (241 tests ✅)
- **spider_crawl_projection** - Procedural spider animation (8 tests ✅)
- **twitching_body** - 3-servo victim animatronic (5 tests ✅)
- **window_spider_trigger** - Video scare system (integration tests ✅)

---

## 🎯 Quick Start for New Agents

### 1. Understand the Structure

```
halloween/                    # ← You are here
├── hatching_egg/            # Reusable: Spider egg animatronic
├── spider_crawl_projection/ # Reusable: Browser animation
├── twitching_body/          # Reusable: Twitching victim
├── window_spider_trigger/   # Reusable: Video trigger
├── 2025/                    # Year-specific: 2025 event materials
│   ├── PROJECT_PLAN.md      # 6-chamber spider haunted house
│   ├── SHOPPING_LIST.md
│   └── retros/
├── README.md                # Main documentation
└── .github/workflows/       # CI tests (ALL PASSING ✅)
```

### 2. Run Tests Immediately

**Before doing anything, verify everything works:**

```bash
# Test each project
cd hatching_egg && pixi run test                  # 241 tests
cd spider_crawl_projection && pixi run test       # 8 tests
cd twitching_body && pixi run integration-test    # 5 tests
cd window_spider_trigger && pixi run integration-test

# Or run CI locally (requires all projects)
# GitHub Actions will run all tests on push
```

### 3. Choose Your Task

**Option A: Work on existing animatronics**
- Read project's `README.md` and `AGENT_HANDOFF.md`
- Run tests first
- Make changes
- Run tests again
- Update project's `CHANGELOG.md`

**Option B: Create new animatronic for next year**
- Study existing projects as templates
- Create new directory in root (e.g., `new_effect/`)
- Use Pixi for environment management
- Write tests from day one
- Follow existing project structure

**Option C: Prepare for new year (e.g., 2026)**
- Create `2026/` directory
- Copy templates from `2025/`
- Update plans for new theme
- Test all animatronics still work

---

## 🧪 Testing Philosophy

**ALL CODE MUST HAVE TESTS - NO EXCEPTIONS**

Each project has comprehensive testing:
- Unit tests for logic
- Integration tests for hardware setup
- CI runs automatically on every push

**If tests fail in CI, DO NOT MERGE.** Fix the tests first.

---

## 🛠️ Development Environment - Pixi

**CRITICAL: ALL projects use Pixi for reproducibility**

```bash
# Install Pixi (one-time)
curl -fsSL https://pixi.sh/install.sh | bash

# Every project has:
cd <project>/
pixi install           # Install dependencies
pixi run test          # Run tests
pixi run deploy        # Deploy to hardware
```

**Why Pixi?**
- Reproducible environments across years
- No system dependencies
- Works on any machine
- Ensures projects work in CI

**Never:**
- Install system packages for project dependencies
- Use system python/node/arduino-cli
- Modify global configuration

---

## 📁 Project Structure Standards

Every animatronic project should follow this structure:

```
project_name/
├── README.md                # Quick start, features, usage
├── AGENT_HANDOFF.md         # Technical details for next agent
├── CHANGELOG.md             # Version history
├── pixi.toml                # Environment + commands
├── arduino/                 # Arduino sketches (if hardware)
│   └── main/main.ino
├── scripts/                 # Shell scripts
│   └── integration_test.sh
├── test*.js                 # Unit tests (Node.js projects)
└── docs/                    # Additional docs (optional)
    ├── TROUBLESHOOTING.md
    └── WIRING.md
```

**Documentation Guidelines:**
- `README.md` - User-facing, focus on "how to use"
- `AGENT_HANDOFF.md` - Technical, focus on "how it works"
- `CHANGELOG.md` - Update with EVERY commit
- Keep docs concise and actionable

---

## 🤖 Hardware Projects (Arduino)

Three projects use **DFRobot Beetle** (Leonardo):
- hatching_egg
- twitching_body
- window_spider_trigger

**Standard Setup Process:**
```bash
pixi run setup              # Install arduino-cli + cores
pixi run arduino-compile    # Verify sketch compiles
pixi run arduino-flash      # Upload to Beetle
pixi run arduino-monitor    # View serial output
```

**Hardware Testing:**
- Integration tests verify setup without hardware
- Manual testing required for actual hardware
- Document hardware setup in project's WIRING.md

---

## 🎨 Browser Projects (Animation)

One project uses browser-based animation:
- spider_crawl_projection

**Standard Development:**
```bash
pixi run serve              # Start HTTP server
pixi run open               # Open in browser
pixi run test               # Run unit tests
```

**Animation Testing:**
- Unit tests for geometry/kinematics
- Visual inspection in browser
- No hardware required

---

## 📊 Continuous Integration

**GitHub Actions runs all tests automatically**

Workflow: `.github/workflows/test.yml`

**What runs in CI:**
1. Hatching Egg - 241 unit tests
2. Spider Crawl - 8 geometry tests
3. Twitching Body - 5 integration tests (setup + compile)
4. Window Spider - 5 integration tests (setup + compile)

**Current Status:** ✅ ALL TESTS PASSING

**If you break CI:**
1. Check the Actions tab on GitHub
2. Read the error logs
3. Fix locally and test: `pixi run test`
4. Push fix

---

## 🗂️ Year-Specific Materials

**2025 Event:** Spider haunted house (6 chambers)
- Location: `2025/`
- Completed: Oct 2025
- All animatronics worked successfully

**For Future Years:**
```bash
# Create new year folder
mkdir 2026
cp 2025/PROJECT_PLAN.md 2026/
cp 2025/SHOPPING_LIST.md 2026/

# Update for new theme
# Test all animatronics still work
# Customize layouts
```

**What goes in year folders:**
- PROJECT_PLAN.md (chamber layouts, theme)
- SHOPPING_LIST.md (budget, materials)
- LAYOUT_MAP.txt (physical layout)
- Scripts/dialogue (tamari_script.txt, etc.)
- Event photos/videos
- Retrospectives

**What stays in root:**
- Reusable animatronics (code + hardware)
- Shared documentation (GIT_LFS_SETUP.md, etc.)
- CI configuration

---

## 🔧 Common Tasks

### Testing Everything
```bash
# Quick verification all projects work
cd hatching_egg && pixi run test && cd ..
cd spider_crawl_projection && pixi run test && cd ..
cd twitching_body && pixi run integration-test && cd ..
cd window_spider_trigger && pixi run integration-test && cd ..
```

### Adding a New Animatronic
1. Create directory in root: `mkdir new_effect/`
2. Add `pixi.toml` (copy from similar project)
3. Create `README.md`, `AGENT_HANDOFF.md`, `CHANGELOG.md`
4. Write code + tests
5. Add to CI workflow (`.github/workflows/test.yml`)
6. Verify CI passes

### Updating Documentation
- Update each project's `CHANGELOG.md` with changes
- Update `README.md` if user-facing changes
- Update `AGENT_HANDOFF.md` if technical changes
- Keep root `README.md` updated with project list

### Git Workflow
```bash
# Make changes
pixi run test               # Verify locally

# Commit
git add -A
git commit -m "Brief description

Detailed explanation of changes.

🤖 Generated with [Claude Code](https://claude.com/claude-code)

Co-Authored-By: Claude <noreply@anthropic.com>"

# Push and verify CI
git push origin main
# Check Actions tab on GitHub
```

---

## 📚 Key Documentation

**Root Level:**
- `README.md` - Repository overview + CI status
- `MIGRATION_2025_TO_MULTI_YEAR.md` - Migration history
- `GIT_LFS_SETUP.md` - Binary file handling

**Each Project:**
- `README.md` - Quick start + features
- `AGENT_HANDOFF.md` - Complete technical details
- `CHANGELOG.md` - Version history (KEEP UPDATED!)

**2025 Event:**
- `2025/PROJECT_PLAN.md` - 6-chamber layout
- `2025/README.md` - Event summary
- `2025/retros/` - Post-event notes

---

## 🚨 Important Notes

### Git LFS
Binary files are tracked with Git LFS:
- `*.mp4` (videos)
- `*.png` (images)
- `*.mp3` (audio)

Don't commit large binaries without LFS!

### Pixi.toml Format
All projects now use `[workspace]` not `[project]`:
```toml
[workspace]  # ✅ Correct
name = "project-name"
```

### Arduino-CLI Setup
Hardware projects need directory setup:
```bash
mkdir -p .pixi/bin .arduino15  # Required before setup
pixi run setup                  # Install arduino-cli
```

### Shell Scripts with set -e
Use `$((var + 1))` not `((var++))`:
```bash
set -e
TESTS_PASSED=$((TESTS_PASSED + 1))  # ✅ Works with set -e
```

---

## 🎯 Current Status (2025-11-06)

### Repository
- ✅ Reorganized for multi-year reuse
- ✅ CI passing (all tests green)
- ✅ No deprecation warnings
- ✅ Ready for next year

### Projects Status
| Project | Tests | Status | Notes |
|---------|-------|--------|-------|
| hatching_egg | 241 ✅ | Complete | Production ready |
| spider_crawl_projection | 8 ✅ | Complete | Keyframe mode removed |
| twitching_body | 5 ✅ | Complete | Production ready |
| window_spider_trigger | 5 ✅ | Complete | Production ready |

### Recent Changes
- Removed unused keyframe feature from spider_crawl_projection
- Fixed CI: arduino-cli setup, integration tests
- Refactored pixi.toml to use `workspace`
- All tests passing in CI

---

## 📖 For Your First Session

**Day 1 Checklist:**
1. ✅ Read this file completely
2. ✅ Read root `README.md`
3. ✅ Check CI badge (should be green)
4. ✅ Run tests on all projects
5. ✅ Read `2025/PROJECT_PLAN.md` for context
6. ✅ Choose a project to work on
7. ✅ Read that project's `AGENT_HANDOFF.md`
8. ✅ Make a small test change
9. ✅ Verify tests pass
10. ✅ Now you're ready to work!

**Resources:**
- [Pixi Documentation](https://pixi.sh)
- [Arduino-CLI Documentation](https://arduino.github.io/arduino-cli/)
- Project READMEs (each has quick start guide)

---

## 💡 Best Practices

✅ **DO:**
- Test immediately after ANY change
- Update CHANGELOG.md with every commit
- Use `pixi run` for all operations
- Keep documentation concise
- Run tests before pushing
- Follow existing project structure
- Read code to understand before changing

❌ **DON'T:**
- Skip testing
- Commit without updating CHANGELOG.md
- Use system tools (always use Pixi)
- Create new docs without good reason
- Break CI (fix before merging)
- Leave unused code
- Guess - read the code!

---

**Last Updated:** 2025-11-06
**Status:** ✅ All systems operational, CI passing
**Next Steps:** Ready for 2026 planning or new animatronics

**For help:** Read project's `AGENT_HANDOFF.md` and `TROUBLESHOOTING.md`
