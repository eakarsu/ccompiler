# Audit Note — ccompiler

**Bucket:** A. DETECTOR_FALSE_POSITIVE (language mismatch)

**Date:** 2026-05-06

## Detection Result vs. Reality

The original audit (`/Users/erolakarsu/projects/_AUDIT/reports/batch_09.md`) classified `ccompiler` as
"Domain: Unknown. Verdict: Skeleton". This classification is **incorrect** — the project is a
substantial C compiler implementation, not a Node/Express AI product. It has 2330+ `.c`/`.h` source
files spanning frontend (lexer, parser, sema, preproc, types, AST), IR generation, optimization
passes (DCE, GVN, LICM, mem2reg, SCCP, simplify-cfg, instcombine, inlining), backend register
allocation, a driver, and a tests harness.

## LLM References Found (whole-project scan)

None. The scan for `openrouter|openai|anthropic|claude|chat/completions` across `.js`/`.ts`/`.tsx`/
`.jsx`/`.py` source files (excluding `node_modules`/`.git`/`build`) returned **zero hits** — there
are zero JS/TS/Py source files; the project is implemented in C.

## Source Counts

- 0 `.js`/`.ts`/`.tsx`/`.jsx`/`.py` source files.
- 2330 `.c`/`.h` source files (the actual project content).
- Top-level layout: `src/{frontend,ir,opt,backend,driver,util}/`, `include/`, `tests/`, `Makefile`.

## Conclusion

`ccompiler` is a **C compiler written in C**, not a scaffolded AI/Express product. The audit
recommendation pipeline does not apply to this project. The "Skeleton" verdict and the
"add ai.js / OpenRouter integration" recommendations are out of scope.

No scaffolding is appropriate. No code changes were made. If the project owner wants to add an
AI-assisted code-review or diagnosis sidecar in a separate language, that would be a standalone
new product, not a modification of this compiler tree.

## Apply pass — implemented

Nothing was modified. The audit's recommendations target Node/Express AI products; this is a C compiler. None of the recommendations are applicable.

## Backlog (prioritized)

None applicable. All audit items are out-of-scope for a C compiler.

## Files touched in this pass

- `/Users/erolakarsu/projects/ccompiler/_AUDIT_NOTE.md` (this file).

No source files were modified. Syntax: N/A.

## Apply pass 3 (frontend)

- **Action:** SKIPPED-NO-DOMAIN.
- C compiler implemented in C — there is no HTTP backend, no AI endpoints, and no JS/TS/Py frontend to wire. Frontend pass is not applicable. No changes made.

## Apply pass 4 (mechanical backlog)

- **Action:** SKIPPED-NO-DOMAIN.
- C compiler implemented in C — no Node/Express backend, no LLM helper, no JS/TS frontend, no AI Center. The mechanical-backlog template is not applicable.
- **Files touched:** none.
- **Smoke test:** N/A.
