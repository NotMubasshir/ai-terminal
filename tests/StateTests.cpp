// SPDX-License-Identifier: Apache-2.0
#include "core/AgentState.hpp"
#include <cassert>
int main_state_tests(){using namespace ai_terminal::core;AgentStateMachine s;assert(s.state()==AgentState::IDLE);assert(s.transition(AgentState::THINKING));assert(s.transition(AgentState::PLANNING));assert(s.transition(AgentState::EXECUTING));assert(s.transition(AgentState::OBSERVING));assert(s.transition(AgentState::VERIFYING));assert(s.transition(AgentState::COMPLETED));assert(!s.transition(AgentState::EXECUTING));return 0;}
