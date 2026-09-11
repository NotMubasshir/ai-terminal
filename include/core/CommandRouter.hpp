// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <string>

namespace ai_terminal::core {
class CommandRouter {
public:
    enum class Kind { UserCommand, AiRequest, BuiltIn, Empty };
    struct Parsed { Kind kind = Kind::Empty; std::string payload; };
    Parsed parse(const std::string& input) const;
};
}
