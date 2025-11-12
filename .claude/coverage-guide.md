# Code Coverage Quick Reference for Agents

## Running Coverage Locally

```bash
# All projects at once
./scripts/run-coverage.sh

# Individual projects using Pixi (RECOMMENDED)
cd window_spider_trigger && pixi run coverage
cd spider_crawl_projection && pixi run coverage
cd hatching_egg && pixi run coverage
```

## Checking Coverage Results

```bash
# View summary in terminal
cat window_spider_trigger/coverage/coverage-summary.json | grep -A 5 total

# Open HTML report using Pixi
cd window_spider_trigger && pixi run view-coverage

# Or open directly
xdg-open window_spider_trigger/coverage/index.html
```

## Coverage Tools by Project

| Project | Languages | Tools | Coverage Command | Status |
|---------|-----------|-------|------------------|--------|
| window_spider_trigger | JS | Jest | `pixi run coverage` | ⚠️ No tests yet |
| spider_crawl_projection | JS | c8 | `pixi run coverage` | ✅ 97.55% |
| hatching_egg | JS + C++ + Python | c8, lcov, coverage.py | `pixi run coverage` | ✅ JS: 92.12%, C++: 171 tests, Python: config |
| twitching_body | Arduino C++ | N/A | No coverage | Hardware-only |

**hatching_egg Multi-Language Commands:**
- `pixi run coverage` - All languages (JS, C++, Python)
- `pixi run coverage-js-only` - JavaScript only
- `pixi run test-cpp-coverage` - C++ only
- `pixi run test-python-coverage` - Python only
- `pixi run view-coverage` - Open all 3 reports
- `pixi run view-coverage-js` - Open JS report only
- `pixi run view-coverage-cpp` - Open C++ report only
- `pixi run view-coverage-python` - Open Python report only

## Improving Coverage

When coverage is low:

1. **Identify uncovered code:**
   ```bash
   # Run coverage
   pixi run coverage

   # Open HTML report to see red/yellow highlighted lines
   pixi run view-coverage
   ```

2. **Add tests for uncovered lines:**
   - Look for untested functions
   - Add test cases for edge cases
   - Test error handling paths

3. **Re-run coverage:**
   ```bash
   pixi run coverage
   ```

## Common Coverage Issues

### "No tests found"
- Ensure test files match pattern: `**/*.test.js` or `**/test-*.js`
- Check that environment is set up: `pixi install`

### "Coverage threshold not met"
- Check jest.config.js for thresholds
- Add more tests to increase coverage
- Or adjust thresholds if appropriate

### "Cannot find module"
- Install dependencies: `pixi install`
- Check that test imports match actual file paths

### "Command not found: npm" or "Command not found: npx"
- All commands should use `pixi run <task>` instead
- Pixi manages the Node.js environment automatically

## Coverage on GitHub

- **SonarCloud:** Quality gate checks on PRs
- **Codecov:** Visual coverage diffs on PRs
- **GitHub Actions:** Workflow runs on every push

## Files Created

- `sonar-project.properties` - SonarCloud configuration
- `.github/workflows/coverage.yml` - GitHub Actions workflow
- `scripts/run-coverage.sh` - Local coverage script
- `COVERAGE.md` - Full coverage documentation
- `jest.config.js` - Jest configuration (per project)
- `c8.config.json` - c8 configuration (per project)
