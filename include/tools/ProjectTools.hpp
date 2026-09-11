// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "tools/Tool.hpp"
#include "terminal/WindowsTerminal.hpp"
#include <filesystem>
#include <memory>
namespace ai_terminal::tools {
std::unique_ptr<Tool> make_detect_project_type_tool(const std::filesystem::path& root);
std::unique_ptr<Tool> make_inspect_project_tool(const std::filesystem::path& root);
std::unique_ptr<Tool> make_build_project_tool(const std::filesystem::path& root,const std::shared_ptr<terminal::WindowsTerminal>& terminal);
std::unique_ptr<Tool> make_test_project_tool(const std::filesystem::path& root,const std::shared_ptr<terminal::WindowsTerminal>& terminal);
}
