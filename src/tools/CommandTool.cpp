// SPDX-License-Identifier: Apache-2.0
#include "tools/CommandTool.hpp"

#include <nlohmann/json.hpp>
#include <utility>

namespace ai_terminal::tools {
namespace {

class CommandBase : public Tool {
public:
    explicit CommandBase(std::shared_ptr<terminal::WindowsTerminal> terminal)
        : terminal_(std::move(terminal)) {}

protected:
    ToolResult run(const nlohmann::json& args, bool powershell) {
        if (!args.contains("command") || !args["command"].is_string()) {
            return {false, "Missing string argument: command."};
        }

        const std::string command = args.at("command").get<std::string>();
        auto result = powershell
                          ? terminal_->execute_powershell(command, 300000)
                          : terminal_->execute_command(command, 300000);

        nlohmann::json output = {
            {"started", result.started},
            {"exit_code", result.exit_code},
            {"timed_out", result.timed_out},
            {"cancelled", result.cancelled},
            {"stdout", result.stdout_text},
            {"stderr", result.stderr_text},
        };

        if (!result.error.empty()) {
            output["error"] = result.error;
        }

        const bool ok = result.started && !result.timed_out &&
                        !result.cancelled && result.error.empty() &&
                        result.exit_code == 0;

        return {ok, output.dump(2), result.exit_code};
    }

    std::shared_ptr<terminal::WindowsTerminal> terminal_;
};

class ExecuteCommand final : public CommandBase {
public:
    using CommandBase::CommandBase;

    std::string name() const override { return "execute_command"; }

    std::string description() const override {
        return "Execute a validated Windows cmd.exe command in the workspace.";
    }

    nlohmann::json schema() const override {
        return {
            {"type", "object"},
            {"properties", {{"command", {{"type", "string"}}}}},
            {"required", {"command"}},
        };
    }

    security::RiskLevel risk() const override {
        return security::RiskLevel::MEDIUM;
    }

    ToolResult execute(const nlohmann::json& args) override {
        return run(args, false);
    }
};

class ExecutePowerShell final : public CommandBase {
public:
    using CommandBase::CommandBase;

    std::string name() const override { return "execute_powershell"; }

    std::string description() const override {
        return "Execute a PowerShell command in the workspace; requires permission.";
    }

    nlohmann::json schema() const override {
        return {
            {"type", "object"},
            {"properties", {{"command", {{"type", "string"}}}}},
            {"required", {"command"}},
        };
    }

    security::RiskLevel risk() const override {
        return security::RiskLevel::MEDIUM;
    }

    ToolResult execute(const nlohmann::json& args) override {
        return run(args, true);
    }
};

} // namespace

std::unique_ptr<Tool> make_execute_command_tool(
    const std::shared_ptr<terminal::WindowsTerminal>& terminal) {
    return std::make_unique<ExecuteCommand>(terminal);
}

std::unique_ptr<Tool> make_execute_powershell_tool(
    const std::shared_ptr<terminal::WindowsTerminal>& terminal) {
    return std::make_unique<ExecutePowerShell>(terminal);
}

} // namespace ai_terminal::tools
