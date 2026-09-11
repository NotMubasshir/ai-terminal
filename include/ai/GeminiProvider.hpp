// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "ai/AIProvider.hpp"
#include <string>
namespace ai_terminal::ai {
class GeminiProvider final : public AIProvider {
public:
    explicit GeminiProvider(std::string model);
    std::string name() const override { return "gemini"; }
    AIResponse send(const std::vector<ChatMessage>& history,const std::string& system_instruction,const std::vector<nlohmann::json>& tools) override;
    AIResponse send_function_results(const std::string& interaction_id,const std::vector<FunctionResultInput>& results,const std::vector<nlohmann::json>& tools,const std::string& system_instruction) override;
private:
    std::string model_;
};
}
