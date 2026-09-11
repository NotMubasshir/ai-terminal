// SPDX-License-Identifier: Apache-2.0
#include "ai/MockProvider.hpp"

#include <utility>

namespace ai_terminal::ai {

MockProvider::MockProvider(std::deque<AIResponse> responses)
    : responses_(std::move(responses)) {}

AIResponse MockProvider::send(const std::vector<ChatMessage>&,
                              const std::string&,
                              const std::vector<nlohmann::json>&) {
    if (responses_.empty()) {
        AIResponse response;
        response.ok = true;
        response.text = "Mock provider complete.";
        response.http_status = 200;
        response.interaction_id = "mock";
        response.requires_action = false;
        return response;
    }

    AIResponse response = std::move(responses_.front());
    responses_.pop_front();
    return response;
}

AIResponse MockProvider::send_function_results(
    const std::string&,
    const std::vector<FunctionResultInput>&,
    const std::vector<nlohmann::json>&,
    const std::string&) {
    // A deterministic mock treats tool results as the next agent turn.
    return send({}, {}, {});
}

} // namespace ai_terminal::ai
