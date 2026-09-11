// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "security/PermissionManager.hpp"
#include <nlohmann/json.hpp>
#include <string>
namespace ai_terminal::tools {
struct ToolResult { bool ok=false; std::string output; long exit_code=-1; bool truncated=false; };
class Tool {
public:
    virtual ~Tool()=default;
    virtual std::string name() const=0;
    virtual std::string description() const=0;
    virtual nlohmann::json schema() const=0;
    virtual security::RiskLevel risk() const=0;
    virtual bool validate(const nlohmann::json& args,std::string& error) const;
    virtual ToolResult execute(const nlohmann::json& args)=0;
};
}
