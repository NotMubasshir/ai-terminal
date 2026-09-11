// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "tools/Tool.hpp"
#include <memory>
namespace ai_terminal::tools {
std::unique_ptr<Tool> make_environment_tool();
std::unique_ptr<Tool> make_system_info_tool();
}
