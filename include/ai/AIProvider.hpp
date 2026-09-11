// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
namespace ai_terminal::ai {
struct ChatMessage { std::string role; std::string text; };
struct FunctionCall { std::string id; std::string name; nlohmann::json arguments = nlohmann::json::object(); };
struct FunctionResultInput { std::string call_id; std::string name; nlohmann::json result; bool is_error=false; };
struct AIResponse { bool ok=false; std::string text; std::string error; long http_status=0; std::string interaction_id; std::vector<FunctionCall> function_calls; bool requires_action=false; };
class AIProvider {
public:
    virtual ~AIProvider()=default;
    virtual std::string name() const=0;
    virtual AIResponse send(const std::vector<ChatMessage>& history,const std::string& system_instruction,const std::vector<nlohmann::json>& tools)=0;
    virtual AIResponse send_function_results(const std::string& interaction_id,const std::vector<FunctionResultInput>& results,const std::vector<nlohmann::json>& tools,const std::string& system_instruction)=0;
};
}
