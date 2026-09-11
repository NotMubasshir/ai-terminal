// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "security/PermissionManager.hpp"
#include <filesystem>
#include <mutex>
#include <string>
#include <nlohmann/json.hpp>

namespace ai_terminal::security {
class AuditLogger {
public:
    AuditLogger(std::filesystem::path path, size_t max_bytes = 10 * 1024 * 1024, size_t max_output_bytes = 64 * 1024);
    void event(const std::string& event_name, const nlohmann::json& data = {});
    void tool_request(const std::string& tool, const nlohmann::json& args, RiskLevel risk);
    void permission(const PermissionRequest& request, Decision decision);
    void execution(const std::string& tool, bool ok, const std::string& output, long exit_code = -1);
    std::filesystem::path path() const { return path_; }
private:
    std::filesystem::path path_;
    size_t max_bytes_;
    size_t max_output_bytes_;
    mutable std::mutex mutex_;
    void rotate_if_needed();
    static std::string timestamp();
    static nlohmann::json redact(nlohmann::json value);
};
}
