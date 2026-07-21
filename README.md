# mycc

`mycc` is an experimental C11 compiler implemented in C. This repository is
retained as an **internal command-line compiler tool**, not as an end-user web
application, service, or deployable product.

The supported product boundary is deliberately small:

- `make` builds `build/debug/mycc` from the project-owned compiler sources.
- `make test` validates the boundary metadata, four unit-test executables, and
  the integration corpus in `tests/integration/`.
- `build/debug/mycc --help` is the supported CLI entry point.
- The compiler may invoke the host assembler and linker when producing objects
  or executables. It does not provide a sandbox for untrusted C programs.

The HTML files and generated assembly samples at the repository root are
diagnostic artifacts. They are not separate applications or supported release
surfaces.

## Build and test

Requirements are a POSIX-like host, `make`, a C11 host compiler, Python 3 for
the metadata check, and GNU `timeout` for the integration runner.

```sh
make
make test
build/debug/mycc --help
```

Release builds use `make BUILD=release`. Sanitizer builds use
`make BUILD=asan`. Installation is intentionally not part of the normal
workflow because `make install` writes to `/usr/local/bin`; use it only after
reviewing the target host and permissions.

## Status and ownership

The tool is retained and actively testable, but remains internal and
experimental. The repository owner and security-patching owner are Erol
Akarsu. The supported baseline and provenance are machine-readable in
[`TOOL_BOUNDARY.json`](TOOL_BOUNDARY.json) and explained in
[`PROVENANCE.md`](PROVENANCE.md).

There is currently no license grant. Do not publish binaries, redistribute the
source, or copy it into another product until the owner adds an explicit
license. See [`SUPPORT.md`](SUPPORT.md), [`SECURITY.md`](SECURITY.md), and
[`docs/RELEASE_PROCESS.md`](docs/RELEASE_PROCESS.md) for the maintenance and
release gates.
