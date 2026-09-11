// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <string>
namespace ai_terminal::core {
enum class AgentState { IDLE, THINKING, PLANNING, AWAITING_PERMISSION, EXECUTING, OBSERVING, VERIFYING, RECOVERING, COMPLETED, FAILED, CANCELLED };
const char* state_name(AgentState state);
class AgentStateMachine {
public:
    AgentState state() const { return state_; }
    bool transition(AgentState next);
    void reset() { state_ = AgentState::IDLE; }
private:
    AgentState state_ = AgentState::IDLE;
};
}
