// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "terminal/WindowsProcess.hpp"
#include <filesystem>
#include <string>
namespace ai_terminal::terminal {
struct CommandResult { int exit_code=-1; std::string stdout_text; std::string stderr_text; bool started=false; bool timed_out=false; bool cancelled=false; std::string error; };
class WindowsTerminal {
public:
    static void request_global_cancel();
    static void clear_global_cancel();
    static bool global_cancel_requested();
    WindowsTerminal(std::filesystem::path working_directory,size_t max_output_bytes=256*1024);
    CommandResult execute_command(const std::string& command,DWORD timeout_ms=300000);
    CommandResult execute_powershell(const std::string& command,DWORD timeout_ms=300000);
    const std::filesystem::path& working_directory() const { return working_directory_; }
    void cancel() { cancelled_=true; }
    void reset_cancel() { cancelled_=false; }
private:
    std::filesystem::path working_directory_;
    size_t max_output_bytes_;
    bool cancelled_=false;
};
}
