// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <windows.h>
#include <functional>
#include <string>
#include <vector>
namespace ai_terminal::terminal {
struct ProcessOptions { std::wstring executable; std::vector<std::wstring> arguments; std::wstring working_directory; DWORD timeout_ms=300000; size_t max_output_bytes=256*1024; std::function<bool()> cancelled; };
struct ProcessResult { bool started=false; bool timed_out=false; bool cancelled=false; int exit_code=-1; std::string stdout_text; std::string stderr_text; std::string error; };
class WindowsProcess { public: static ProcessResult run(const ProcessOptions& options); };
}
