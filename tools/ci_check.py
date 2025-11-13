#!/usr/bin/env python3
"""
GitHub Actions CI Status Checker

Checks the status of recent GitHub Actions workflow runs.

Usage:
    python tools/ci_check.py
    python tools/ci_check.py --limit 10
    python tools/ci_check.py --workflow coverage.yml
"""

import subprocess
import json
import sys
import argparse
from typing import List, Dict, Any
from datetime import datetime


class CIChecker:
    """GitHub Actions CI status checker."""

    def __init__(self):
        self.repo_owner = None
        self.repo_name = None
        self._detect_repo()

    def _detect_repo(self):
        """Detect repository from git remote."""
        try:
            result = subprocess.run(
                ['git', 'remote', 'get-url', 'origin'],
                capture_output=True,
                text=True,
                check=True
            )
            remote_url = result.stdout.strip()

            # Parse GitHub URL
            if 'github.com' in remote_url:
                # Handle both SSH and HTTPS URLs
                if remote_url.startswith('git@github.com:'):
                    parts = remote_url.replace('git@github.com:', '').replace('.git', '').split('/')
                elif 'github.com/' in remote_url:
                    parts = remote_url.split('github.com/')[-1].replace('.git', '').split('/')
                else:
                    raise ValueError("Cannot parse GitHub URL")

                self.repo_owner = parts[0]
                self.repo_name = parts[1]
            else:
                raise ValueError("Not a GitHub repository")

        except (subprocess.CalledProcessError, ValueError) as e:
            print(f"Error: Unable to detect GitHub repository: {e}", file=sys.stderr)
            print("Make sure you're in a git repository with a GitHub remote.", file=sys.stderr)
            sys.exit(1)

    def check_gh_cli(self) -> bool:
        """Check if gh CLI is available."""
        try:
            subprocess.run(['gh', '--version'], capture_output=True, check=True)
            return True
        except (subprocess.CalledProcessError, FileNotFoundError):
            return False

    def get_workflow_runs_gh(self, limit: int = 5, workflow: str = None) -> List[Dict[str, Any]]:
        """Get workflow runs using gh CLI."""
        cmd = ['gh', 'run', 'list', '--json',
               'databaseId,name,status,conclusion,createdAt,updatedAt,headBranch,event',
               '--limit', str(limit)]

        if workflow:
            cmd.extend(['--workflow', workflow])

        try:
            result = subprocess.run(cmd, capture_output=True, text=True, check=True)
            return json.loads(result.stdout)
        except subprocess.CalledProcessError as e:
            print(f"Error running gh CLI: {e}", file=sys.stderr)
            print("Make sure gh CLI is installed and authenticated.", file=sys.stderr)
            sys.exit(1)

    def format_run_status(self, run: Dict[str, Any]) -> str:
        """Format a single workflow run status."""
        status = run.get('status', 'unknown')
        conclusion = run.get('conclusion', 'unknown')

        # Determine icon and status text
        if status == 'completed':
            if conclusion == 'success':
                icon = "✅"
                status_text = "SUCCESS"
            elif conclusion == 'failure':
                icon = "❌"
                status_text = "FAILED"
            elif conclusion == 'cancelled':
                icon = "⚠️"
                status_text = "CANCELLED"
            elif conclusion == 'skipped':
                icon = "⚠️"
                status_text = "SKIPPED"
            else:
                icon = "❓"
                status_text = conclusion.upper()
        elif status == 'in_progress':
            icon = "🔄"
            status_text = "IN PROGRESS"
        elif status == 'queued':
            icon = "⏳"
            status_text = "QUEUED"
        else:
            icon = "❓"
            status_text = status.upper()

        # Format timestamp
        created_at = run.get('createdAt', '')
        try:
            dt = datetime.fromisoformat(created_at.replace('Z', '+00:00'))
            time_str = dt.strftime('%Y-%m-%d %H:%M:%S')
        except:
            time_str = created_at

        name = run.get('name', 'Unknown')
        branch = run.get('headBranch', 'unknown')
        event = run.get('event', 'unknown')
        run_id = run.get('databaseId', '?')

        return f"{icon} {status_text} - {name} (#{run_id})\n   Branch: {branch} | Event: {event} | Time: {time_str}"

    def format_status_report(self, limit: int = 5, workflow: str = None) -> str:
        """Generate formatted CI status report."""
        lines = []

        lines.append("=" * 70)
        lines.append("GitHub Actions CI Status")
        lines.append("=" * 70)
        lines.append(f"Repository: {self.repo_owner}/{self.repo_name}")
        if workflow:
            lines.append(f"Workflow: {workflow}")
        lines.append("")

        runs = self.get_workflow_runs_gh(limit, workflow)

        if not runs:
            lines.append("No workflow runs found.")
            lines.append("")
            lines.append("=" * 70)
            return "\n".join(lines)

        lines.append(f"Recent Runs (showing {len(runs)}):")
        lines.append("-" * 70)

        success_count = 0
        failure_count = 0
        other_count = 0

        for run in runs:
            lines.append(self.format_run_status(run))

            conclusion = run.get('conclusion', '')
            if conclusion == 'success':
                success_count += 1
            elif conclusion == 'failure':
                failure_count += 1
            else:
                other_count += 1

        lines.append("")
        lines.append("Summary:")
        lines.append("-" * 70)
        lines.append(f"  Successful: {success_count}")
        lines.append(f"  Failed: {failure_count}")
        lines.append(f"  Other: {other_count}")
        lines.append("")
        lines.append("=" * 70)

        return "\n".join(lines)


def main():
    parser = argparse.ArgumentParser(
        description='Check GitHub Actions CI status',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Check recent runs (default: 5)
  python tools/ci_check.py

  # Check more runs
  python tools/ci_check.py --limit 10

  # Check specific workflow
  python tools/ci_check.py --workflow coverage.yml
        """
    )
    parser.add_argument('--limit', type=int, default=5, help='Number of runs to show (default: 5)')
    parser.add_argument('--workflow', help='Filter by workflow name (e.g., coverage.yml)')

    args = parser.parse_args()

    try:
        checker = CIChecker()

        # Check if gh CLI is available
        if not checker.check_gh_cli():
            print("Error: gh CLI not found", file=sys.stderr)
            print("Install with: https://cli.github.com/", file=sys.stderr)
            return 1

        report = checker.format_status_report(args.limit, args.workflow)
        print(report)

        # Check if most recent run failed
        runs = checker.get_workflow_runs_gh(1, args.workflow)
        if runs and runs[0].get('conclusion') == 'failure':
            return 1

        return 0

    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        import traceback
        traceback.print_exc()
        return 1


if __name__ == '__main__':
    sys.exit(main())
