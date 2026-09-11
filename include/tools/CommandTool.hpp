// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "tools/Tool.hpp"
#include "terminal/WindowsTerminal.hpp"
#include <memory>
namespace ai_terminal::tools {
std::unique_ptr<Tool> make_execute_command_tool(const std::shared_ptr<terminal::WindowsTerminal>& terminal);
std::unique_ptr<Tool> make_execute_powershell_tool(const std::shared_ptr<terminal::WindowsTerminal>& terminal);
}
