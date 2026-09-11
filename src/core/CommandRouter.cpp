// SPDX-License-Identifier: Apache-2.0

#include "core/CommandRouter.hpp"

#include <cctype>

namespace ai_terminal::core {
CommandRouter::Parsed CommandRouter::parse(const std::string& input) const {
    size_t start = 0;
    while (start < input.size() && std::isspace(static_cast<unsigned char>(input[start]))) ++start;
    if (start == input.size()) return {Kind::Empty, {}};
    const std::string trimmed = input.substr(start);
    if (trimmed.rfind("/ai ", 0) == 0) return {Kind::AiRequest, trimmed.substr(4)};
    if (!trimmed.empty() && trimmed[0] == '/') return {Kind::BuiltIn, trimmed};
    return {Kind::UserCommand, trimmed};
}
}
