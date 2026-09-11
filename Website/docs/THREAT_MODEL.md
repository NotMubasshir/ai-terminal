# Threat Model

| Threat | Attack surface | Mitigation | Remaining risk |
|---|---|---|---|
| Malicious model output | Function calls, arguments | Tool allowlist, schema validation, deterministic policy | A model can request legitimate dangerous tools and wait for approval |
| Prompt injection | README/source/comments/tool output | Untrusted-content policy and strict tool boundary | Model may still be socially manipulated into requesting an allowed action |
| Command injection | cmd/PowerShell/Git | Separate tools, permission boundary, conservative Git argument validation | Approved shell commands retain shell semantics |
| Workspace escape | File tools | Relative paths, canonicalization, reparse-point rejection | Windows filesystem races and unusual filesystem providers require continued testing |
| Credential leakage | Environment/files/audit | Allowlisted environment, secret redaction, no API-key config storage | A user can explicitly ask to read an ordinary credential file; policy must be strengthened for deployment-specific secret stores |
| Malicious dependency | Build tools | User approval, real build output, documentation | Approved build systems may execute package/build scripts |
| Unsafe Git operations | Git tools | Dedicated operations and high-risk permission | User approval can still intentionally rewrite project state |
| Process abuse | Process tools/commands | Timeouts, Job Objects, permissions | A permitted process can consume CPU/memory until timeout |
| Resource exhaustion | Files/search/output/agent loop | Configurable size, call, runtime and round limits | Windows-level resources are not fully quota-controlled |
| Tool abuse | Dispatcher | Single security choke point, unknown-tool rejection, audit | Implementation bugs could create a bypass; security tests and review remain necessary |
| TOCTOU | Filesystem | Revalidation at execution, reparse checks | No user-mode filesystem API completely eliminates every race without stronger sandboxing |
