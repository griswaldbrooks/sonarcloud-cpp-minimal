# Setup Instructions

## SonarCloud Configuration

1. **Go to SonarCloud:** https://sonarcloud.io/

2. **Add New Project:**
   - Click "+" in top right → "Analyze new project"
   - Select "griswaldbrooks/sonarcloud-cpp-minimal"
   - Choose "With GitHub Actions"

3. **Get the SONAR_TOKEN:**
   - SonarCloud will display a token
   - Copy this token

4. **Add Token to GitHub Secrets:**
   ```bash
   # Method 1: Via GitHub CLI
   gh secret set SONAR_TOKEN --body "YOUR_TOKEN_HERE" --repo griswaldbrooks/sonarcloud-cpp-minimal

   # Method 2: Via Web UI
   # Go to: https://github.com/griswaldbrooks/sonarcloud-cpp-minimal/settings/secrets/actions
   # Click "New repository secret"
   # Name: SONAR_TOKEN
   # Value: [paste token]
   ```

5. **Verify Configuration:**
   - Organization: griswaldbrooks
   - Project Key: griswaldbrooks_sonarcloud-cpp-minimal
   - Token added to GitHub secrets

6. **Trigger CI:**
   ```bash
   # Make a small change and push, or re-run workflow
   git commit --allow-empty -m "Trigger CI for SonarCloud"
   git push
   ```

7. **Monitor Progress:**
   - GitHub Actions: https://github.com/griswaldbrooks/sonarcloud-cpp-minimal/actions
   - SonarCloud Dashboard: https://sonarcloud.io/dashboard?id=griswaldbrooks_sonarcloud-cpp-minimal

## Expected Timeline

1. **Immediate:** GitHub Actions starts (30-60 seconds)
2. **2-3 minutes:** Build and test complete
3. **3-5 minutes:** gcovr generates coverage.xml
4. **5-10 minutes:** SonarCloud analysis complete
5. **10+ minutes:** SonarCloud dashboard updates

## Verification

Once CI completes, verify with the API tool:

```bash
cd /home/griswald/personal/halloween
python tools/sonarcloud_verify.py \
    --project griswaldbrooks_sonarcloud-cpp-minimal \
    --component src
```

Expected output:
- Component: src/math.cpp
- Coverage: 100%
- Lines: 4/4

## What We're Testing

**If coverage works:**
- We know the basic approach (CMake + gcovr + SonarCloud) is sound
- The issue is specific to our monorepo or paths

**If coverage fails:**
- We know something fundamental is wrong
- We can share this minimal repo with SonarCloud support
- We need to investigate the gcovr → SonarCloud integration

## Troubleshooting

### CI Fails
Check GitHub Actions logs for errors in:
- CMake configuration
- Build step
- Test execution
- gcovr coverage generation
- SonarCloud upload

### SonarCloud Shows 0% Coverage
Run verification tool to see what SonarCloud actually has:
```bash
python /home/griswald/personal/halloween/tools/sonarcloud_verify.py \
    --project griswaldbrooks_sonarcloud-cpp-minimal
```

Look for:
- Are files listed?
- Do they have coverage data?
- Are paths correct?

### GitHub Actions Missing Secrets
If CI fails with "SONAR_TOKEN not found":
1. Verify secret was added: https://github.com/griswaldbrooks/sonarcloud-cpp-minimal/settings/secrets/actions
2. Re-run the workflow after adding secret
