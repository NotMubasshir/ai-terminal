// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "tools/Tool.hpp"
#include <memory>
namespace ai_terminal::tools {
std::unique_ptr<Tool> make_list_processes_tool();
std::unique_ptr<Tool> make_process_info_tool();
std::unique_ptr<Tool> make_terminate_process_tool();
}
