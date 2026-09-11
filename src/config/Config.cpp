// SPDX-License-Identifier: Apache-2.0
#include "config/Config.hpp"
#include <fstream>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <windows.h>
namespace ai_terminal::config {
const char* permission_mode_name(PermissionMode m){switch(m){case PermissionMode::AlwaysAsk:return "always_ask";case PermissionMode::DangerousOnly:return "dangerous_only";case PermissionMode::SafeAuto:return "safe_auto";case PermissionMode::Session:return "session";case PermissionMode::AlwaysDeny:return "always_deny";}return "dangerous_only";}
bool parse_permission_mode(const std::string& s,PermissionMode&m){if(s=="always_ask")m=PermissionMode::AlwaysAsk;else if(s=="dangerous_only")m=PermissionMode::DangerousOnly;else if(s=="safe_auto")m=PermissionMode::SafeAuto;else if(s=="session")m=PermissionMode::Session;else if(s=="always_deny")m=PermissionMode::AlwaysDeny;else return false;return true;}
namespace {std::filesystem::path app_data(){wchar_t b[MAX_PATH]{};auto n=GetEnvironmentVariableW(L"APPDATA",b,MAX_PATH);return n&&n<MAX_PATH?std::filesystem::path(b)/"AI-Terminal":std::filesystem::current_path()/".ai-terminal";}}
std::filesystem::path ConfigStore::config_path(){return app_data()/"config.json";}
Config ConfigStore::load(){Config c;c.working_directory=std::filesystem::current_path();std::ifstream f(config_path());if(f)try{nlohmann::json j;f>>j;c.provider=j.value("provider",c.provider);c.model=j.value("model",c.model);c.color_output=j.value("color_output",c.color_output);c.safe_mode=j.value("safe_mode",c.safe_mode);c.audit_logging=j.value("audit_logging",c.audit_logging);auto pm=j.value("permission_mode",std::string(permission_mode_name(c.permission_mode)));parse_permission_mode(pm,c.permission_mode);auto cwd=j.value("working_directory","");if(!cwd.empty())c.working_directory=cwd;auto&l=c.limits;l.max_agent_rounds=j.value("max_agent_rounds",l.max_agent_rounds);l.max_tool_calls=j.value("max_tool_calls",l.max_tool_calls);l.max_retries=j.value("max_retries",l.max_retries);l.max_files_changed=j.value("max_files_changed",l.max_files_changed);l.max_runtime_seconds=j.value("max_runtime_seconds",l.max_runtime_seconds);l.max_tool_output_bytes=j.value("max_tool_output_bytes",l.max_tool_output_bytes);l.max_read_file_bytes=j.value("max_read_file_bytes",l.max_read_file_bytes);l.max_write_file_bytes=j.value("max_write_file_bytes",l.max_write_file_bytes);l.max_context_bytes=j.value("max_context_bytes",l.max_context_bytes);}catch(...){c=Config{};c.working_directory=std::filesystem::current_path();}char* safe_mode = nullptr;
size_t safe_mode_size = 0;
if(_dupenv_s(&safe_mode, &safe_mode_size, "AI_TERMINAL_SAFE_MODE") == 0 && safe_mode && *safe_mode) {
    c.safe_mode = true;
    c.permission_mode = PermissionMode::AlwaysAsk;
}
free(safe_mode); return c;}
bool ConfigStore::save(const Config&c,std::string&e){try{auto p=config_path();std::filesystem::create_directories(p.parent_path());nlohmann::json j={{"provider",c.provider},{"model",c.model},{"working_directory",c.working_directory.string()},{"color_output",c.color_output},{"safe_mode",c.safe_mode},{"audit_logging",c.audit_logging},{"permission_mode",permission_mode_name(c.permission_mode)},{"max_agent_rounds",c.limits.max_agent_rounds},{"max_tool_calls",c.limits.max_tool_calls},{"max_retries",c.limits.max_retries},{"max_files_changed",c.limits.max_files_changed},{"max_runtime_seconds",c.limits.max_runtime_seconds},{"max_tool_output_bytes",c.limits.max_tool_output_bytes},{"max_read_file_bytes",c.limits.max_read_file_bytes},{"max_write_file_bytes",c.limits.max_write_file_bytes},{"max_context_bytes",c.limits.max_context_bytes}};std::ofstream o(p,std::ios::trunc);if(!o){e="Unable to write "+p.string();return false;}o<<j.dump(2)<<'\n';return true;}catch(const std::exception&x){e=x.what();return false;}}
}
