# Development

## Windows build

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug --parallel
ctest --test-dir build -C Debug --output-on-failure
cmake --build build --config Release --parallel
ctest --test-dir build -C Release --output-on-failure
cmake --build build --config Release --target PACKAGE
```

## CI

The GitHub Actions workflow runs configure, Release build, CTest and CPack on `windows-2022`.

## Rules

- Keep the dispatcher as the only AI-to-tool execution boundary.
- Never execute model prose as a shell command.
- Never add API keys to source, fixtures or CI.
- Add a regression test for security bugs.
- Prefer standard library/Windows APIs over dependencies unless a dependency materially improves correctness.
- Do not claim Windows behavior is verified unless Windows CI or a real Windows environment has verified it.
