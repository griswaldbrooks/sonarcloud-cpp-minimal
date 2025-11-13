# MCP Tool Investigation Results

## Date: 2025-11-12

## Question
Can we create MCP (Model Context Protocol) tools in the `.claude/` directory for automated tool integration?

## Investigation

### What We Found

**`.claude/` directory structure:**
```
.claude/
├── agents/                    # Agent definitions (coder, reviewer, etc.)
│   ├── coder.md
│   ├── code-reviewer.md
│   ├── project-manager.md
│   └── test-runner.md
├── claude.md                  # Main project instructions
├── coverage-guide.md          # Coverage guidelines
├── NEXT_AGENT.md             # Handoff documentation
└── settings.local.json        # Permissions configuration
```

**Permissions model (settings.local.json):**
- Defines allowed Bash commands
- Whitelists specific command patterns
- Controls WebFetch and WebSearch access
- No MCP tool definition format found

### Conclusion

**MCP tools are NOT supported in the current Claude Code `.claude/` directory structure.**

The `.claude/` directory serves a different purpose:
1. **Agent definitions** - Specialized agent prompts/instructions
2. **Project instructions** - CLAUDE.md for context
3. **Permissions** - What commands Claude can execute
4. **Documentation** - Handoff guides and quick references

## Alternative Approach: Pixi Tasks

Instead of MCP tools, we use **Pixi tasks** for tool integration:

### pixi.toml Integration

```toml
[tasks]
# Status checks
check-ci = "python tools/ci_check.py"
check-sonarcloud = "python tools/sonarcloud_check.py --project griswaldbrooks_sonarcloud-cpp-minimal"
status-check = "python tools/status.py"
```

### Usage

```bash
# Claude can invoke these directly
pixi run status-check

# Or humans can use them
pixi run check-ci
pixi run check-sonarcloud
```

### Advantages of Pixi Tasks

1. **Integrated with environment** - Uses pixi's managed dependencies
2. **Versioned** - pixi.toml is in version control
3. **Documented** - `pixi task list` shows all available tasks
4. **Cross-platform** - Works on any system with pixi
5. **No special permissions needed** - Just regular commands

## Recommendation

**Use Pixi tasks for tool integration, NOT MCP tools.**

### For this project:

```bash
# Quick status check
pixi run status-check

# Individual checks
pixi run check-ci
pixi run check-sonarcloud

# Direct Python invocation also works
python tools/status.py
```

### For future projects:

1. Create tools in `tools/` directory
2. Make them executable (`chmod +x`)
3. Add to `pixi.toml` as tasks
4. Document in `tools/README.md`
5. Reference in main `README.md`

## Future MCP Support?

If Claude Code adds MCP tool support in the future, the tool structure would likely be:

```
.claude/tools/
├── check-ci.json
├── check-sonarcloud.json
└── status.json
```

Each with a definition like:
```json
{
  "name": "check-ci",
  "description": "Check GitHub Actions CI status",
  "command": "python3 tools/ci_check.py",
  "arguments": [
    {
      "name": "limit",
      "type": "integer",
      "description": "Number of runs to show",
      "default": 5
    }
  ]
}
```

**But this is NOT currently supported.**

## What We Built Instead

### 1. Python Tools (tools/ directory)
- `sonarcloud_check.py` - SonarCloud status
- `ci_check.py` - GitHub Actions status
- `status.py` - Combined status check

### 2. Pixi Integration (pixi.toml)
- Tasks defined for each tool
- Easy invocation: `pixi run <task>`
- Integrated with project environment

### 3. Documentation (tools/README.md)
- Comprehensive usage guide
- Examples for each tool
- Troubleshooting section
- Integration patterns

## Summary

**MCP tools:** Not supported in `.claude/` directory
**Alternative:** Pixi tasks + Python scripts
**Result:** Fully functional tool integration without MCP
**Documentation:** Complete in tools/README.md

---

**Investigated by:** Claude Code Agent
**Date:** 2025-11-12
**Conclusion:** Use Pixi tasks for tool integration
**Status:** Complete - Tools working as designed
