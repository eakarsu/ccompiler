# Completeness Review: ccompiler

**Review date:** 2026-07-18

## Assessment basis

Static inspection of project-owned source and configuration only; no dependency installation, build, database migration, external-service call, or runtime launch was performed. The scan considered 1000 project files (961 source files), 0 manifest(s), 952 test-like file(s), and 0 CI workflow(s), excluding dependency/generated directories.

## Classification

**Not an app**

This folder is best treated as source material, a library/tool, generated workspace, dependency cache, or portfolio container—not as an independently complete application workflow app. App-completeness criteria therefore do not apply until a supported executable product boundary is defined.

## Why it is not a complete app

- No clear, independently supported end-user application boundary was identified in the inspected source/configuration.
- Ownership, release target, supported entry point, and acceptance criteria are absent or belong to an upstream/reference project.

## Needed features

1. Decide whether to retain this as an upstream/reference dependency, internal tool, archive, or source for extraction.
2. Document provenance, license, owner, supported version, update strategy, and security-patching responsibility.
3. If an app is intended, create a separate product boundary with an explicit entry point, user journey, configuration contract, tests, and release process.

## Risks or launch blockers

- Accidental deployment or unsupported modification could create security, licensing, and maintenance obligations.
- Treating this folder as an original product may obscure upstream provenance and update responsibility.

## Evidence inspected

- `codex-custom-viz-and-ops.html:15`
- `src/frontend/parser.c:396`
- `src/driver/main.c`
- `tests/integration/test_077_hash_apps.c`
- `tests/gen_tests.sh`

## Recommended next action

Record an explicit retain/extract/archive decision; only create an app roadmap if a supported product boundary and owner are assigned.

## Implementation progress

Implemented the review on 2026-07-19 without misclassifying this compiler as a
web application:

- Recorded the explicit `retain-as-internal-tool` decision and the supported
  compiler CLI boundary in `TOOL_BOUNDARY.json` and `README.md`. The supported
  entry points are now `make`, `make test`, and `build/debug/mycc`; root HTML
  and assembly artifacts are explicitly excluded from the product boundary.
- Documented repository provenance, the owner, reviewed commit and source
  manifest digest, third-party import policy, support/update strategy, and
  security-patching responsibility in `PROVENANCE.md`, `SUPPORT.md`, and
  `SECURITY.md`.
- Made the absent license an explicit external release gate instead of
  inventing a license grant. Source redistribution, binary publication, and
  product extraction remain blocked until the owner chooses a license.
- Added a concrete internal release/extraction checklist and a separate-product
  contract for any future web/hosted application in
  `docs/RELEASE_PROCESS.md`.
- Added dependency-free boundary validation to `make test` and a read-only CI
  workflow. Validation passed with four unit tests and 683 integration tests
  (687 total compiler tests) on the reviewed host; the compiler built
  successfully. Existing compiler warnings remain engineering debt, but did
  not prevent the documented experimental CLI boundary from passing its test
  corpus.

## Runtime and login acceptance — 2026-07-20

- **Status:** NOT_APPLICABLE
- **Startup safety:** the retained internal compiler-tool boundary and its completed dependency-free validation were inspected.
- **Startup and primary journey:** N/A for the application gate; this is a supported CLI/tool boundary, not an independently supported web application.
- **Readiness and login:** N/A; the tool exposes no browser server, user identity, or login workflow.
- **Browser/server evidence:** N/A; no application server exists.
- **Cleanup:** no runtime or disposable service was created for this gate.
- **Residual issue:** any future hosted product requires a separate owned application boundary and its own runtime/login acceptance.
