// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "tools/Tool.hpp"
#include "terminal/WindowsTerminal.hpp"
#include <filesystem>
#include <memory>
namespace ai_terminal::tools {
std::unique_ptr<Tool> make_git_tool(std::string operation,const std::filesystem::path& root,const std::shared_ptr<terminal::WindowsTerminal>& terminal);
}
