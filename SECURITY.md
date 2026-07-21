# Security policy

The compiler and its generated programs are **not a security sandbox**.
Treat C source, include paths, object files, linker inputs, and produced
executables as potentially hostile. Build and run untrusted inputs only in a
disposable, least-privileged environment without secrets or production mounts.

Do not run `make install` in CI or on shared hosts. The normal validation path
builds only below the ignored `build/` directory. Review changes to the driver,
preprocessor include resolution, process execution, assembler/linker arguments,
and output paths as security-sensitive.

Erol Akarsu is responsible for triage and patching. Report suspected issues
privately to the repository owner (for example, with a private GitHub security
advisory) and include a minimal reproducer, affected commit, host architecture,
and compiler command. Do not publish a reproducer before the owner has assessed
the issue.

No public security-support window is promised while the repository is
unlicensed and experimental. A public release requires the gates in
`docs/RELEASE_PROCESS.md`.
