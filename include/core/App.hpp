// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "ai/AIProvider.hpp"
#include "config/Config.hpp"
#include "core/AgentState.hpp"
#include "core/CommandRouter.hpp"
#include "security/AuditLogger.hpp"
#include "security/PermissionManager.hpp"
#include "terminal/WindowsTerminal.hpp"
#include "tools/ToolDispatcher.hpp"
#include "tools/ToolRegistry.hpp"
#include <memory>
#include <string>
#include <vector>
namespace ai_terminal::core {
class App {
public:
    App();
    int run();
private:
    void rebuild_workspace();
    void create_provider();
    void handle_ai(const std::string& prompt);
    void handle_builtin(const std::string& command);
    void print_banner() const;
    void print_help() const;
    void print_tools() const;
    void print_history() const;
    void print_config() const;
    bool set_config_value(const std::string& key,const std::string& value);
    std::string system_instruction() const;
    config::Config config_;
    CommandRouter router_;
    std::shared_ptr<terminal::WindowsTerminal> terminal_;
    security::PermissionManager permissions_;
    std::unique_ptr<security::AuditLogger> audit_;
    tools::ToolRegistry tools_;
    std::unique_ptr<tools::ToolDispatcher> dispatcher_;
    std::unique_ptr<ai::AIProvider> provider_;
    std::vector<ai::ChatMessage> history_;
    AgentStateMachine state_;
    bool running_=true;
};
}
