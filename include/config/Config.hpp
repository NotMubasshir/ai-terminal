// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <filesystem>
#include <string>

namespace ai_terminal::config {

enum class PermissionMode { AlwaysAsk, DangerousOnly, SafeAuto, Session, AlwaysDeny };
const char* permission_mode_name(PermissionMode mode);
bool parse_permission_mode(const std::string& value, PermissionMode& mode);

struct AgentLimits {
    int max_agent_rounds = 20;
    int max_tool_calls = 80;
    int max_retries = 3;
    int max_files_changed = 50;
    int max_runtime_seconds = 900;
    size_t max_tool_output_bytes = 256 * 1024;
    size_t max_read_file_bytes = 2 * 1024 * 1024;
    size_t max_write_file_bytes = 4 * 1024 * 1024;
    size_t max_context_bytes = 512 * 1024;
};

struct Config {
    std::string provider = "gemini";
    std::string model = "gemini-3.8-flash";
    std::filesystem::path working_directory{};
    bool color_output = true;
    bool safe_mode = false;
    PermissionMode permission_mode = PermissionMode::DangerousOnly;
    bool audit_logging = true;
    std::filesystem::path audit_log{};
    AgentLimits limits{};
};

class ConfigStore {
public:
    static Config load();
    static bool save(const Config& config, std::string& error);
    static std::filesystem::path config_path();
};

} // namespace ai_terminal::config
