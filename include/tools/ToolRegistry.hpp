// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "tools/Tool.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace ai_terminal::tools {
class ToolRegistry {
public:
    void add(std::unique_ptr<Tool> tool);
    Tool* find(const std::string& name) const;
    std::vector<nlohmann::json> gemini_declarations() const;
    std::vector<std::string> names() const;
private:
    std::unordered_map<std::string,std::unique_ptr<Tool>> tools_;
};
}
