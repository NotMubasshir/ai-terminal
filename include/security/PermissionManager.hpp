// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "config/Config.hpp"
#include <functional>
#include <string>
#include <unordered_set>
#include <nlohmann/json.hpp>

namespace ai_terminal::security {

enum class RiskLevel { SAFE, LOW, MEDIUM, HIGH, CRITICAL, BLOCKED };
const char* risk_name(RiskLevel risk);

struct PermissionRequest {
    std::string tool_name;
    RiskLevel risk = RiskLevel::BLOCKED;
    nlohmann::json arguments = nlohmann::json::object();
    std::string reason;
    std::string affected_resources;
};

enum class Decision { Deny, AllowOnce, AllowSession, AlwaysDeny };
const char* decision_name(Decision d);

class PermissionManager {
public:
    using ConfirmFn = std::function<Decision(const PermissionRequest&)>;
    explicit PermissionManager(ConfirmFn confirm = {});
    void set_mode(config::PermissionMode mode);
    void set_safe_mode(bool enabled);
    void reset_session();
    RiskLevel classify(const std::string& tool_name, const nlohmann::json& args = {}) const;
    Decision authorize(const PermissionRequest& request);
    bool is_session_allowed(const std::string& tool_name) const;
private:
    ConfirmFn confirm_;
    config::PermissionMode mode_ = config::PermissionMode::DangerousOnly;
    bool safe_mode_ = false;
    std::unordered_set<std::string> session_allowed_;
    std::unordered_set<std::string> always_denied_;
};

} // namespace ai_terminal::security
