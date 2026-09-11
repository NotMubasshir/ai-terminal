# Architecture

```text
Console UI
   |
   v
Agent Runtime / State Machine
   |
   +---- Context + bounded history
   |
   +---- AIProvider
   |       +-- GeminiProvider
   |       +-- MockProvider
   |
   v
Structured Function Call
   |
   v
ToolDispatcher  <--- security choke point
   |
   +--> schema/argument validation
   +--> deterministic risk classification
   +--> permission policy
   +--> audit logging
   |
   v
Registered Tools
   +--> Filesystem / patching
   +--> cmd / PowerShell
   +--> Project detection/build/test
   +--> Git
   +--> Environment/system/processes
   |
   v
Windows Process / Filesystem APIs
```

## Agent state machine

The runtime uses explicit states:

`IDLE -> THINKING -> PLANNING -> AWAITING_PERMISSION -> EXECUTING -> OBSERVING -> VERIFYING -> COMPLETED`

Failure and cancellation paths transition through `RECOVERING`, `FAILED` or `CANCELLED` and are bounded by configured limits.

## Tool contract

Each tool provides:

- name
- description
- JSON parameter schema
- deterministic risk classification
- argument validation
- execution
- structured result

The AI provider never receives direct access to tool implementations.

## Provider contract

`AIProvider` abstracts conversation turns and structured function results. Gemini-specific HTTP and JSON handling is isolated in `GeminiProvider` so the agent runtime can be tested with `MockProvider`.

## Windows process model

`WindowsProcess` creates child processes with `CreateProcessW`, captures output in temporary files, enforces a timeout, checks a cancellation callback and uses a Windows Job Object with `JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE` where available.

## Workspace model

Every AI filesystem tool owns a workspace guard. Relative paths are canonicalized immediately before use, checked for escape from the workspace and rejected when any existing path component is a Windows reparse point.
