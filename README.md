# AI Terminal

**AI Terminal 1.0.0** is a Windows 10/11 x64, security-first AI coding and automation terminal written in C++20.

It is designed as an **agent runtime**, not a chatbot wrapped around a shell:

```text
USER GOAL
  -> AI PROVIDER
  -> PLAN / TOOL CALL
  -> SCHEMA VALIDATION
  -> SECURITY POLICY
  -> USER PERMISSION
  -> TOOL DISPATCH
  -> REAL WINDOWS EXECUTION
  -> OBSERVATION
  -> VERIFICATION / RECOVERY
  -> RESULT
```

The model is untrusted. The application owns tool registration, validation, risk classification, permissions, workspace boundaries, execution limits and audit logging.

## What is implemented in 1.0.0

- Gemini Interactions API provider with structured function calling.
- Deterministic mock provider for provider-independent tests.
- Explicit agent state machine and bounded multi-step loop.
- Central `ToolDispatcher` security choke point.
- JSON schema-style required/type/unknown-argument validation.
- Workspace-constrained filesystem operations.
- Reparse-point/junction checks for AI-controlled paths.
- Safe exact-context patching with ambiguity rejection.
- Command and PowerShell execution with bounded output and Windows Job Object containment.
- Process timeout and cancellation propagation for child processes.
- Project detection for CMake, Visual Studio/MSBuild, Ninja, Python, Node.js, Rust and Git markers.
- Real CMake/MSBuild/Ninja build execution and CTest execution where detected.
- Git status/diff/log/branch plus permission-protected add/commit/checkout operations.
- Allow-once, allow-session, safe-auto, always-ask and always-deny permission modes.
- Safe mode that blocks medium/high/critical actions.
- Environment inspection with a strict allowlist and secret omission.
- Windows process inspection and permission-protected termination.
- Persistent JSONL audit logging with redaction and rotation.
- Configurable agent/tool/resource limits.
- Windows CI, CTest and CPack ZIP packaging.

## Requirements

- Windows 10/11 x64
- Visual Studio 2022+ with Desktop development with C++
- CMake 3.23+
- Git
- Gemini API key for the Gemini provider

## Build

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release --parallel
```

## Test

```powershell
ctest --test-dir build -C Release --output-on-failure
```

## Run

```powershell
.\build\Release\AI-Terminal.exe
```

Command-line options:

```text
AI-Terminal --help
AI-Terminal --version
AI-Terminal --config
AI-Terminal --reset
AI-Terminal --safe-mode
```

## Gemini configuration

Set the API key in the Windows user environment:

```powershell
[Environment]::SetEnvironmentVariable("GEMINI_API_KEY", "YOUR_API_KEY", "User")
```

The key is never written to the application configuration and is excluded from environment inspection.

AI Terminal uses Google's Gemini Interactions API. The provider sends registered function declarations, receives structured `function_call` steps, executes them locally through the dispatcher, and sends `function_result` steps back to Gemini.

## Terminal commands

```text
/help
/version
/provider
/model
/pwd
/tools
/history
/config
/config model <name>
/config provider gemini|mock
/config cwd <path>
/config permission_mode always_ask|dangerous_only|safe_auto|session|always_deny
/config safe_mode on|off
/config color on|off
/safe-mode on|off
/reset
/clear
/exit
/ai <natural-language goal>
```

Non-`/` input is a **user-authored local command**. It is not treated as model output and is executed directly for convenience. AI-generated commands can only execute through registered tools and the security boundary.

## Core tool inventory

| Tool | Default risk | Purpose |
|---|---:|---|
| `get_working_directory` | SAFE | Workspace path |
| `set_working_directory` | MEDIUM | Change workspace |
| `list_directory` | SAFE | Directory listing |
| `read_file` | LOW | Bounded file read |
| `create_file` | LOW | Create-only file write |
| `write_file` | MEDIUM | File write |
| `apply_patch` | MEDIUM | Exact-context edit |
| `create_directory` | LOW | Directory creation |
| `delete_file` | HIGH | Destructive deletion |
| `copy_file` | MEDIUM | File copy |
| `move_file` | MEDIUM | File move |
| `search_files` | LOW | Literal source search |
| `file_exists` | SAFE | Existence check |
| `get_file_info` | SAFE | Metadata |
| `execute_command` | MEDIUM | Windows `cmd.exe` |
| `execute_powershell` | MEDIUM | PowerShell |
| `build_project` | MEDIUM | Detected build system |
| `test_project` | MEDIUM | CTest |
| `get_environment` | SAFE | Allowlisted environment |
| `get_system_info` | SAFE | Windows system information |
| `list_processes` | SAFE | Process list |
| `get_process_info` | LOW | Process metadata |
| `terminate_process` | HIGH | Process termination |
| `detect_project_type` | SAFE | Evidence-based detection |
| `inspect_project` | SAFE | Project summary |
| `git_status` | SAFE | Git status |
| `git_diff` | SAFE | Git diff |
| `git_log` | SAFE | Git log |
| `git_branch` | SAFE | Branch list |
| `git_add` | HIGH | Stage changes |
| `git_commit` | HIGH | Commit changes |
| `git_checkout` | HIGH | Change branch |

Blocked tool names such as `format_disk`, `disable_security` and `extract_credentials` are rejected by application policy.

## Security model

### AI output is untrusted

The model cannot execute arbitrary text. It can only request a registered tool with structured JSON arguments.

Every request goes through:

```text
Tool lookup
 -> argument validation
 -> deterministic risk classification
 -> policy / safe-mode evaluation
 -> user permission
 -> execution
 -> bounded result
 -> audit log
```

### Filesystem boundary

AI filesystem paths must be relative to the configured workspace. Absolute paths, traversal escapes and existing Windows reparse-point components are rejected. Validation occurs immediately before execution rather than relying on a prompt instruction.

### Command boundary

Commands are executed as Windows processes with explicit executable boundaries for `cmd.exe` and PowerShell. Long-running processes are time-bounded and attached to a Windows Job Object where available so child processes are cleaned up with the parent operation.

Git arguments that reach a shell command are restricted to a conservative character set. Arbitrary model-provided shell fragments are not accepted as Git parameters.

### Prompt injection

Repository content, source comments, README files, Git metadata, command output and tool results are data. They do not acquire instruction authority merely because they contain imperative text.

### Secrets

The Gemini key is loaded from the process environment only. Environment inspection uses an allowlist and explicitly omits the key. Audit logging redacts fields whose names indicate keys, tokens, passwords, secrets or credentials.

### Limits

Safe defaults bound:

- agent rounds
- tool calls
- retries
- modified-file scope at the policy layer
- command runtime
- tool output
- file reads/writes
- context size

## Example workflow

When implemented functionality is used for a real CMake project, a request such as:

```text
/ai Analyze this project, fix the build, run the tests, and explain what changed.
```

can cause the agent to inspect the project, detect CMake, build it, inspect real compiler output, request permission before modifying files, apply an exact-context patch, rebuild and run CTest. The UI must only report build/test success when the corresponding tool returned success.

## Configuration

The configuration file is stored under `%APPDATA%\AI-Terminal\config.json`. It contains provider/model/workspace/policy/limit settings, but not API keys.

## Documentation

- `docs/ARCHITECTURE.md` — runtime and subsystem architecture
- `docs/SECURITY.md` — enforcement model
- `docs/THREAT_MODEL.md` — threat analysis and residual risks
- `docs/DEVELOPMENT.md` — Windows build/test/release workflow
- `docs/ROADMAP.md` — post-1.0 work
- `docs/providers.md` — provider contract
- `CHANGELOG.md` — release history

## Verification status of this source package

The implementation was audited and edited in the supplied source archive. The available execution environment for this work is not Windows and does not contain MSVC/Windows SDK tooling, so a Windows compile, Windows runtime session, Gemini network call and Windows CPack artifact are **NOT VERIFIED IN THIS ENVIRONMENT**. The GitHub Actions workflow is the authoritative Windows build/test verification path.

## License

Apache License 2.0. See `LICENSE`.
