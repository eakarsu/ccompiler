# Support policy

`mycc` is retained as an internal experimental compiler tool. Erol Akarsu owns
technical decisions, baseline updates, and security patches.

## Supported workflow

Changes are supported when they preserve the CLI boundary in `README.md` and
pass `make test`. A baseline update must be an owner-reviewed commit and must
update `TOOL_BOUNDARY.json` if entry points, scope, ownership, or the release
posture changes.

Compiler behavior not represented by the checked-in unit and integration
tests is not a compatibility promise. The checked-in `advanced`, `stress`, and
numbered suite corpora are development material unless they are explicitly
added to the supported `make test` gate.

There is no scheduled upstream synchronization because this is not recorded as
a fork. Third-party code must not be copied in without provenance and license
review. Unsupported or abandoned revisions should be archived rather than
silently deployed.
