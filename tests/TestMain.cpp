// SPDX-License-Identifier: Apache-2.0
#include <iostream>
int main_config_tests(); int main_permission_tests(); int main_tool_registry_tests(); int main_state_tests(); int main_validator_tests();
int main(){std::cout<<"AI Terminal v1.0.0 tests starting...\n";main_config_tests();main_permission_tests();main_tool_registry_tests();main_state_tests();main_validator_tests();std::cout<<"All tests passed.\n";return 0;}
