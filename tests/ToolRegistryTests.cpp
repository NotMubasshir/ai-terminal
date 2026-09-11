// SPDX-License-Identifier: Apache-2.0
#include "tools/ToolRegistry.hpp"
#include "ai/MockProvider.hpp"
#include <deque>
#include <cassert>
class Dummy final:public ai_terminal::tools::Tool{public:std::string name()const override{return"dummy";}std::string description()const override{return"dummy";}nlohmann::json schema()const override{return{{"type","object"},{"properties",nlohmann::json::object()}};}ai_terminal::security::RiskLevel risk()const override{return ai_terminal::security::RiskLevel::SAFE;}ai_terminal::tools::ToolResult execute(const nlohmann::json&)override{return{true,"ok"};}};
int main_tool_registry_tests(){
    ai_terminal::ai::MockProvider mock(std::deque<ai_terminal::ai::AIResponse>{});
    auto mock_response = mock.send({}, "", {});
    assert(mock_response.ok);ai_terminal::tools::ToolRegistry r;r.add(std::make_unique<Dummy>());assert(r.find("dummy"));assert(!r.find("missing"));auto d=r.gemini_declarations();assert(d.size()==1&&d[0]["name"]=="dummy");return 0;}
