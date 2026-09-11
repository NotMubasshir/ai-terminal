// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "security/AuditLogger.hpp"
#include "security/PermissionManager.hpp"
#include "tools/ToolRegistry.hpp"
#include <functional>
namespace ai_terminal::tools {
struct DispatchResult { bool ok=false; bool denied=false; bool cancelled=false; std::string output; };
class ToolDispatcher {
public:
    using PermissionUi = std::function<security::Decision(const security::PermissionRequest&)>;
    ToolDispatcher(ToolRegistry& registry,security::PermissionManager& permissions,security::AuditLogger& audit,const config::AgentLimits& limits);
    DispatchResult dispatch(const std::string& name,const nlohmann::json& args,const std::string& reason={});
    void cancel() { cancelled_=true; }
    void reset_cancel() { cancelled_=false; calls_=0; files_changed_=0; }
private:
    ToolRegistry& registry_;
    security::PermissionManager& permissions_;
    security::AuditLogger& audit_;
    const config::AgentLimits& limits_;
    bool cancelled_=false;
    size_t calls_=0;
    size_t files_changed_=0;
    static std::string bound(const std::string& text,size_t max,bool& truncated);
};
}
