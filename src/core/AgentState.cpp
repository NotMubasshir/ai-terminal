// SPDX-License-Identifier: Apache-2.0
#include "core/AgentState.hpp"
namespace ai_terminal::core {
const char* state_name(AgentState s){switch(s){case AgentState::IDLE:return "IDLE";case AgentState::THINKING:return "THINKING";case AgentState::PLANNING:return "PLANNING";case AgentState::AWAITING_PERMISSION:return "AWAITING_PERMISSION";case AgentState::EXECUTING:return "EXECUTING";case AgentState::OBSERVING:return "OBSERVING";case AgentState::VERIFYING:return "VERIFYING";case AgentState::RECOVERING:return "RECOVERING";case AgentState::COMPLETED:return "COMPLETED";case AgentState::FAILED:return "FAILED";case AgentState::CANCELLED:return "CANCELLED";}return "UNKNOWN";}
bool AgentStateMachine::transition(AgentState n){
    if(n==state_) return true;
    switch(state_){
        case AgentState::IDLE: return n==AgentState::THINKING||n==AgentState::CANCELLED;
        case AgentState::THINKING: return n==AgentState::PLANNING||n==AgentState::AWAITING_PERMISSION||n==AgentState::RECOVERING||n==AgentState::FAILED||n==AgentState::CANCELLED;
        case AgentState::PLANNING: return n==AgentState::AWAITING_PERMISSION||n==AgentState::EXECUTING||n==AgentState::FAILED||n==AgentState::CANCELLED;
        case AgentState::AWAITING_PERMISSION: return n==AgentState::EXECUTING||n==AgentState::FAILED||n==AgentState::CANCELLED||n==AgentState::THINKING;
        case AgentState::EXECUTING: return n==AgentState::OBSERVING||n==AgentState::FAILED||n==AgentState::CANCELLED;
        case AgentState::OBSERVING: return n==AgentState::VERIFYING||n==AgentState::THINKING||n==AgentState::RECOVERING||n==AgentState::FAILED||n==AgentState::CANCELLED;
        case AgentState::VERIFYING: return n==AgentState::COMPLETED||n==AgentState::THINKING||n==AgentState::RECOVERING||n==AgentState::FAILED||n==AgentState::CANCELLED;
        case AgentState::RECOVERING: return n==AgentState::THINKING||n==AgentState::AWAITING_PERMISSION||n==AgentState::FAILED||n==AgentState::CANCELLED;
        case AgentState::COMPLETED: return n==AgentState::IDLE;
        case AgentState::FAILED: return n==AgentState::IDLE;
        case AgentState::CANCELLED: return n==AgentState::IDLE;
    }
    return false;
}
}
