#!/usr/bin/env python3
"""
SonarCloud Status Check Tool

Quick status check for SonarCloud analysis state.
Simplified version of sonarcloud_verify.py for rapid checks.

Usage:
    python tools/sonarcloud_check.py --project griswaldbrooks_sonarcloud-cpp-minimal
    python tools/sonarcloud_check.py --project griswaldbrooks_sonarcloud-cpp-minimal --component src
"""

import requests
import argparse
import sys
from typing import Dict, Any, List, Optional
from datetime import datetime


class SonarCloudChecker:
    """Quick SonarCloud status checker."""

    BASE_URL = "https://sonarcloud.io/api"

    def __init__(self, organization: str, project_key: str):
        self.organization = organization
        self.project_key = project_key

    def _get(self, endpoint: str, params: Dict[str, Any]) -> Dict[str, Any]:
        """Make a GET request to SonarCloud API."""
        url = f"{self.BASE_URL}/{endpoint}"
        try:
            response = requests.get(url, params=params, timeout=10)
            response.raise_for_status()
            return response.json()
        except requests.exceptions.RequestException as e:
            print(f"Error: Failed to communicate with SonarCloud API: {e}", file=sys.stderr)
            sys.exit(1)

    def get_quality_gate_status(self) -> Dict[str, Any]:
        """Get quality gate status."""
        params = {'projectKey': self.project_key}
        data = self._get('qualitygates/project_status', params)
        return data.get('projectStatus', {})

    def get_project_measures(self) -> Dict[str, str]:
        """Get key project metrics."""
        params = {
            'component': self.project_key,
            'metricKeys': 'coverage,bugs,vulnerabilities,code_smells,security_hotspots,lines'
        }
        data = self._get('measures/component', params)
        component = data.get('component', {})
        measures = {m['metric']: m.get('value', '0') for m in component.get('measures', [])}
        return measures

    def get_recent_analysis(self) -> Optional[Dict[str, Any]]:
        """Get most recent analysis."""
        params = {
            'project': self.project_key,
            'ps': 1
        }
        data = self._get('project_analyses/search', params)
        analyses = data.get('analyses', [])
        return analyses[0] if analyses else None

    def get_coverage_detail(self, component_filter: Optional[str] = None) -> List[Dict[str, Any]]:
        """Get detailed coverage for files."""
        params = {
            'component': self.project_key,
            'metricKeys': 'coverage,lines_to_cover,uncovered_lines',
            'qualifiers': 'FIL',
            'ps': 500
        }
        data = self._get('measures/component_tree', params)
        components = data.get('components', [])

        if component_filter:
            components = [c for c in components if c.get('path', '').startswith(component_filter)]

        return components

    def format_status_report(self, component: Optional[str] = None) -> str:
        """Generate formatted status report."""
        lines = []

        # Header
        lines.append("=" * 70)
        lines.append("SonarCloud Status Check")
        lines.append("=" * 70)
        lines.append(f"Project: {self.project_key}")
        if component:
            lines.append(f"Component: {component}")
        lines.append("")

        # Recent analysis
        analysis = self.get_recent_analysis()
        if analysis:
            date_str = analysis.get('date', 'Unknown')
            try:
                dt = datetime.fromisoformat(date_str.replace('Z', '+00:00'))
                date_str = dt.strftime('%Y-%m-%d %H:%M:%S UTC')
            except:
                pass
            lines.append(f"Last Analysis: {date_str}")
            lines.append("")

        # Quality gate
        qg_status = self.get_quality_gate_status()
        status = qg_status.get('status', 'UNKNOWN')
        status_icon = "✅" if status == "OK" else "❌" if status == "ERROR" else "⚠️"
        lines.append(f"Quality Gate: {status_icon} {status}")
        lines.append("")

        # Key metrics
        measures = self.get_project_measures()
        lines.append("Project Metrics:")
        lines.append("-" * 70)

        # Coverage
        coverage = measures.get('coverage', 'N/A')
        if coverage != 'N/A':
            cov_val = float(coverage)
            cov_icon = "✅" if cov_val >= 80 else "⚠️" if cov_val >= 60 else "❌"
            lines.append(f"  Coverage: {cov_icon} {coverage}%")
        else:
            lines.append(f"  Coverage: ❌ No data")

        # Issues
        bugs = measures.get('bugs', '0')
        vulnerabilities = measures.get('vulnerabilities', '0')
        code_smells = measures.get('code_smells', '0')
        hotspots = measures.get('security_hotspots', '0')

        bugs_icon = "✅" if bugs == '0' else "❌"
        vuln_icon = "✅" if vulnerabilities == '0' else "❌"

        lines.append(f"  Bugs: {bugs_icon} {bugs}")
        lines.append(f"  Vulnerabilities: {vuln_icon} {vulnerabilities}")
        lines.append(f"  Code Smells: {code_smells}")
        lines.append(f"  Security Hotspots: {hotspots}")
        lines.append(f"  Lines of Code: {measures.get('lines', '0')}")
        lines.append("")

        # Coverage details
        coverage_detail = self.get_coverage_detail(component)
        if coverage_detail:
            lines.append("File Coverage:")
            lines.append("-" * 70)

            files_with_coverage = []
            files_without_coverage = []

            for file_info in coverage_detail:
                file_measures = {m['metric']: m.get('value') for m in file_info.get('measures', [])}
                path = file_info.get('path', 'Unknown')

                if 'coverage' in file_measures:
                    cov = float(file_measures['coverage'])
                    cov_icon = "✅" if cov >= 80 else "⚠️" if cov >= 60 else "❌"
                    lines_to_cover = file_measures.get('lines_to_cover', '?')
                    uncovered = file_measures.get('uncovered_lines', '?')
                    files_with_coverage.append(f"  {cov_icon} {path}: {cov:.1f}% ({lines_to_cover} lines, {uncovered} uncovered)")
                else:
                    files_without_coverage.append(f"  ❌ {path}: No coverage data")

            if files_with_coverage:
                for line in sorted(files_with_coverage):
                    lines.append(line)

            if files_without_coverage:
                if files_with_coverage:
                    lines.append("")
                for line in sorted(files_without_coverage):
                    lines.append(line)

            lines.append("")
            lines.append(f"Total Files: {len(coverage_detail)}")
            lines.append(f"With Coverage: {len(files_with_coverage)}")
            lines.append(f"Without Coverage: {len(files_without_coverage)}")
        else:
            lines.append("No files found in analysis")

        lines.append("")
        lines.append("=" * 70)

        return "\n".join(lines)


def main():
    parser = argparse.ArgumentParser(
        description='Check SonarCloud project status',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Check entire project
  python tools/sonarcloud_check.py --project griswaldbrooks_sonarcloud-cpp-minimal

  # Check specific component
  python tools/sonarcloud_check.py --project griswaldbrooks_sonarcloud-cpp-minimal --component src
        """
    )
    parser.add_argument('--project', required=True, help='SonarCloud project key')
    parser.add_argument('--organization', default='griswaldbrooks', help='SonarCloud organization')
    parser.add_argument('--component', help='Specific component to check (e.g., src)')

    args = parser.parse_args()

    try:
        checker = SonarCloudChecker(args.organization, args.project)
        report = checker.format_status_report(args.component)
        print(report)

        # Exit with error code if quality gate failed
        qg_status = checker.get_quality_gate_status()
        if qg_status.get('status') == 'ERROR':
            return 1
        return 0

    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        import traceback
        traceback.print_exc()
        return 1


if __name__ == '__main__':
    sys.exit(main())
