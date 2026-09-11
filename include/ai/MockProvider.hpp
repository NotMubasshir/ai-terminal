// SPDX-License-Identifier: Apache-2.0
#pragma once

#include "ai/AIProvider.hpp"

#include <deque>

namespace ai_terminal::ai {

class MockProvider final : public AIProvider {
public:
    explicit MockProvider(std::deque<AIResponse> responses);

    std::string name() const override { return "mock"; }

    AIResponse send(const std::vector<ChatMessage>& history,
                    const std::string& system_instruction,
                    const std::vector<nlohmann::json>& tools) override;

    AIResponse send_function_results(
        const std::string& interaction_id,
        const std::vector<FunctionResultInput>& results,
        const std::vector<nlohmann::json>& tools,
        const std::string& system_instruction) override;

private:
    std::deque<AIResponse> responses_;
};

} // namespace ai_terminal::ai
