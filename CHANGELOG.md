# Changelog

## 1.0.0 — 2026-09-11

### Added

- Explicit agent state machine.
- Central tool dispatcher and argument validation.
- Gemini provider abstraction plus deterministic mock provider.
- Expanded Windows filesystem, command, PowerShell, process, environment, project and Git tools.
- Exact-context patch editing.
- CMake/MSBuild/Ninja build execution and CTest execution.
- Permission modes and safe mode.
- Persistent redacted audit logging with rotation.
- Bounded process execution with Job Object containment.
- Configurable resource limits.
- Threat model and security documentation.
- Expanded unit/security-oriented tests and Windows CI packaging.

### Security

- AI-controlled paths remain workspace constrained at execution time.
- Reparse-point components are rejected for AI filesystem operations.
- Unknown tools and unknown JSON arguments are rejected.
- Destructive/process/Git mutation operations require application-side authorization.
- Environment inspection is allowlisted and excludes the Gemini API key.

### Verification

Windows compilation and runtime verification are delegated to the Windows CI workflow because the development environment used to prepare this source package does not contain MSVC or the Windows SDK.

## 0.2.0

Previous agent foundation release.
