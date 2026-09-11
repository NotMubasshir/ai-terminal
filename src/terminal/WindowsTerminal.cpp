// SPDX-License-Identifier: Apache-2.0
#include "terminal/WindowsTerminal.hpp"
#include <windows.h>
#include <atomic>
namespace ai_terminal::terminal {namespace { std::atomic_bool g_cancel{false};std::wstring w(const std::string&s){if(s.empty())return{};int n=MultiByteToWideChar(CP_UTF8,0,s.data(),(int)s.size(),nullptr,0);std::wstring o(n,L'\0');MultiByteToWideChar(CP_UTF8,0,s.data(),(int)s.size(),o.data(),n);return o;}}
void WindowsTerminal::request_global_cancel(){g_cancel.store(true);}
void WindowsTerminal::clear_global_cancel(){g_cancel.store(false);}
bool WindowsTerminal::global_cancel_requested(){return g_cancel.load();}
WindowsTerminal::WindowsTerminal(std::filesystem::path p,size_t m):working_directory_(std::move(p)),max_output_bytes_(m){}
CommandResult WindowsTerminal::execute_command(const std::string&c,DWORD timeout){ProcessOptions o;o.executable=L"cmd.exe";o.arguments={L"/D",L"/S",L"/C",w(c)};o.working_directory=working_directory_.wstring();o.timeout_ms=timeout;o.max_output_bytes=max_output_bytes_;o.cancelled=[this]{return cancelled_||g_cancel.load();};auto r=WindowsProcess::run(o);return {r.exit_code,r.stdout_text,r.stderr_text,r.started,r.timed_out,r.cancelled,r.error};}
CommandResult WindowsTerminal::execute_powershell(const std::string&c,DWORD timeout){ProcessOptions o;o.executable=L"powershell.exe";o.arguments={L"-NoLogo",L"-NoProfile",L"-NonInteractive",L"-ExecutionPolicy",L"Bypass",L"-Command",w(c)};o.working_directory=working_directory_.wstring();o.timeout_ms=timeout;o.max_output_bytes=max_output_bytes_;o.cancelled=[this]{return cancelled_||g_cancel.load();};auto r=WindowsProcess::run(o);return {r.exit_code,r.stdout_text,r.stderr_text,r.started,r.timed_out,r.cancelled,r.error};}
}
