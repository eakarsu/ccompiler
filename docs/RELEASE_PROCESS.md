# Internal release and extraction process

The current decision is to retain `mycc` as an internal CLI tool. It is not an
application and no separate GUI or service is implied.

Before tagging an internal baseline:

1. Review the diff, provenance of newly imported code, and security-sensitive
   compiler/process changes.
2. Run `make clean && make test` on each host architecture claimed by the tag.
3. Run `make BUILD=asan test` on a supported sanitizer host.
4. Update the commit and source-manifest digest in `TOOL_BOUNDARY.json`.
5. Record known unsupported language features and architecture limitations.

External distribution or extraction into another product additionally
requires an explicit license, an owner-approved versioning policy, release
artifacts built in CI with checksums, and a published vulnerability-reporting
contact. Until those gates are satisfied, release status remains
`blocked-pending-license`.

If a web application, hosted service, or graphical product is later desired,
create it in a separate repository or top-level product directory. It must
define its own entry point, user journey, configuration and secrets contract,
authentication and authorization boundary, tests, deployment process, and
support owner. It must consume a versioned compiler interface rather than
silently treating the diagnostic HTML files as an application.
