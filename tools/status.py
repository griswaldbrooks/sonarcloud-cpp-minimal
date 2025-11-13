#!/usr/bin/env python3
"""
Combined Project Status Checker

Runs both CI and SonarCloud checks for a comprehensive status report.

Usage:
    python tools/status.py
    python tools/status.py --project griswaldbrooks_sonarcloud-cpp-minimal
"""

import sys
import argparse
import subprocess
from pathlib import Path


def print_section_header(title: str):
    """Print a formatted section header."""
    print("\n")
    print("=" * 70)
    print(f"  {title}")
    print("=" * 70)


def run_tool(tool_path: str, args: list) -> int:
    """Run a tool and return its exit code."""
    try:
        result = subprocess.run(
            ['python3', tool_path] + args,
            check=False
        )
        return result.returncode
    except Exception as e:
        print(f"Error running {tool_path}: {e}", file=sys.stderr)
        return 1


def main():
    parser = argparse.ArgumentParser(
        description='Check overall project status (CI + SonarCloud)',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Check everything with defaults
  python tools/status.py

  # Specify project explicitly
  python tools/status.py --project griswaldbrooks_sonarcloud-cpp-minimal

  # More CI runs
  python tools/status.py --ci-limit 10
        """
    )
    parser.add_argument('--project',
                       default='griswaldbrooks_sonarcloud-cpp-minimal',
                       help='SonarCloud project key (default: griswaldbrooks_sonarcloud-cpp-minimal)')
    parser.add_argument('--organization',
                       default='griswaldbrooks',
                       help='SonarCloud organization (default: griswaldbrooks)')
    parser.add_argument('--ci-limit',
                       type=int,
                       default=5,
                       help='Number of CI runs to show (default: 5)')
    parser.add_argument('--skip-ci',
                       action='store_true',
                       help='Skip CI check')
    parser.add_argument('--skip-sonarcloud',
                       action='store_true',
                       help='Skip SonarCloud check')

    args = parser.parse_args()

    tools_dir = Path(__file__).parent
    exit_codes = []

    # Header
    print("=" * 70)
    print("PROJECT STATUS CHECK")
    print("=" * 70)
    print(f"Project: {args.project}")
    print("=" * 70)

    # Check CI status
    if not args.skip_ci:
        print_section_header("GITHUB ACTIONS CI")
        ci_tool = tools_dir / 'ci_check.py'
        ci_exit = run_tool(str(ci_tool), ['--limit', str(args.ci_limit)])
        exit_codes.append(ci_exit)

    # Check SonarCloud status
    if not args.skip_sonarcloud:
        print_section_header("SONARCLOUD ANALYSIS")
        sonar_tool = tools_dir / 'sonarcloud_check.py'
        sonar_exit = run_tool(str(sonar_tool), [
            '--project', args.project,
            '--organization', args.organization
        ])
        exit_codes.append(sonar_exit)

    # Summary
    print("\n")
    print("=" * 70)
    print("OVERALL STATUS")
    print("=" * 70)

    all_passed = all(code == 0 for code in exit_codes)

    if all_passed:
        print("✅ All checks passed!")
        print("")
        print("=" * 70)
        return 0
    else:
        print("❌ Some checks failed!")
        print("")
        if not args.skip_ci:
            ci_status = "✅ PASSED" if exit_codes[0] == 0 else "❌ FAILED"
            print(f"  CI: {ci_status}")
        if not args.skip_sonarcloud:
            sonar_idx = 0 if args.skip_ci else 1
            sonar_status = "✅ PASSED" if exit_codes[sonar_idx] == 0 else "❌ FAILED"
            print(f"  SonarCloud: {sonar_status}")
        print("")
        print("=" * 70)
        return 1


if __name__ == '__main__':
    sys.exit(main())
