// SPDX-License-Identifier: Apache-2.0
#include "security/PermissionManager.hpp"
#include <cassert>
int main_permission_tests(){using namespace ai_terminal::security;PermissionManager p([](const PermissionRequest&){return Decision::AllowOnce;});assert(p.classify("list_directory")==RiskLevel::SAFE);assert(p.classify("read_file")==RiskLevel::LOW);assert(p.classify("write_file")==RiskLevel::MEDIUM);assert(p.classify("delete_file")==RiskLevel::HIGH);assert(p.classify("format_disk")==RiskLevel::BLOCKED);PermissionRequest r{"delete_file",RiskLevel::HIGH,{},"test",""};assert(p.authorize(r)==Decision::AllowOnce);return 0;}
