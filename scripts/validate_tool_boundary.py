#!/usr/bin/env python3
"""Dependency-free validation for the documented internal-tool boundary."""

import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


def main() -> None:
    data = json.loads((ROOT / "TOOL_BOUNDARY.json").read_text(encoding="utf-8"))

    assert data["decision"] == "retain-as-internal-tool"
    assert data["application_intended"] is False
    assert data["owner"]
    assert data["security_patch_owner"]
    assert data["repository"].startswith("https://github.com/")
    assert len(data["supported_baseline"]["git_commit"]) == 40
    assert len(data["supported_baseline"]["source_manifest_sha256"]) == 64
    assert data["supported_entry_points"] == {
        "build": "make",
        "test": "make test",
        "cli": "build/debug/mycc",
    }
    assert data["release_status"] == "blocked-pending-license"
    assert data["license"]["status"] == "not-declared"

    for relative in (
        "README.md",
        "PROVENANCE.md",
        "SUPPORT.md",
        "SECURITY.md",
        "docs/RELEASE_PROCESS.md",
        "Makefile",
        "src/driver/main.c",
        "tests/run_tests.sh",
    ):
        assert (ROOT / relative).is_file(), f"missing boundary file: {relative}"

    readme = (ROOT / "README.md").read_text(encoding="utf-8")
    assert "internal command-line compiler tool" in readme
    assert "no license grant" in readme

    print("tool-boundary metadata: PASS")


if __name__ == "__main__":
    main()
