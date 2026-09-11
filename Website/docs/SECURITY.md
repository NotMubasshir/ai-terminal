# Security Model

AI Terminal treats model output and repository content as untrusted input.

## Enforcement order

```text
structured tool call
 -> registered tool lookup
 -> JSON argument validation
 -> path/command/tool-specific validation
 -> deterministic risk classification
 -> permission policy
 -> execution
 -> bounded output
 -> audit event
```

No prompt instruction can bypass this sequence.

## Permission modes

- `always_ask`: ask for every non-safe operation.
- `dangerous_only`: automatically allow safe/low/medium operations; ask for high-risk operations.
- `safe_auto`: automatically allow safe/low; ask for higher-risk operations.
- `session`: approve medium-or-lower tools for the session after the policy decision.
- `always_deny`: deny every non-safe operation.

Safe mode additionally denies medium, high and critical operations.

## Filesystem

AI filesystem paths must be relative to the workspace. Canonicalization and relative-path containment are checked at execution time. Existing reparse-point components are rejected to reduce junction/symlink escape risk.

Exact-context patching refuses to apply if the expected context is absent or appears more than once.

## Commands

`cmd.exe` and PowerShell are separate registered tools and require policy authorization. Processes have output and runtime limits and are attached to a Job Object where possible.

The Git layer does not accept arbitrary Git command strings; it exposes specific operations and restricts arguments for operations that are assembled into shell commands.

## Secrets

The Gemini API key is read from `GEMINI_API_KEY` and never written to configuration. Environment inspection is allowlisted. Audit logging redacts common secret-bearing field names and obvious Gemini key prefixes.

## Prompt injection

Content such as `IGNORE PREVIOUS INSTRUCTIONS` inside a repository is treated as data. The model is explicitly instructed to treat tool results and repository content as untrusted, but the security boundary does not depend on that instruction.

## Residual risk

This is a local automation agent. Approved commands can still perform harmful actions within the permissions granted by Windows and the user. Security controls reduce accidental and model-driven abuse; they do not turn an authorized Windows process into a sandbox equivalent to a VM.
