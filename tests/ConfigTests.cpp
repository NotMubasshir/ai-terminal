// SPDX-License-Identifier: Apache-2.0
#include "config/Config.hpp"
#include <cassert>
int main_config_tests(){ai_terminal::config::PermissionMode m;assert(ai_terminal::config::parse_permission_mode("dangerous_only",m));assert(m==ai_terminal::config::PermissionMode::DangerousOnly);assert(!ai_terminal::config::parse_permission_mode("invalid",m));auto c=ai_terminal::config::Config{};assert(c.limits.max_tool_calls>0);return 0;}
