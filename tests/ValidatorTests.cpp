// SPDX-License-Identifier: Apache-2.0
#include "tools/ToolRegistry.hpp"
#include <cassert>
class V final:public ai_terminal::tools::Tool{public:std::string name()const override{return"v";}std::string description()const override{return"v";}nlohmann::json schema()const override{return{{"type","object"},{"properties",{{"x",{{"type","string"}}}}},{"required",{"x"}}};}ai_terminal::security::RiskLevel risk()const override{return ai_terminal::security::RiskLevel::SAFE;}ai_terminal::tools::ToolResult execute(const nlohmann::json&)override{return{true,"ok"};}};
int main_validator_tests(){V v;std::string e;assert(v.validate({{"x","ok"}},e));assert(!v.validate({{}},e));assert(!v.validate({{"x",1}},e));assert(!v.validate({{"x","ok"},{"extra",1}},e));return 0;}
