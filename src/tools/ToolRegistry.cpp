// SPDX-License-Identifier: Apache-2.0
#include "tools/ToolRegistry.hpp"
#include <algorithm>
namespace ai_terminal::tools {
void ToolRegistry::add(std::unique_ptr<Tool> t){if(t)tools_[t->name()]=std::move(t);}
Tool* ToolRegistry::find(const std::string& n)const{auto i=tools_.find(n);return i==tools_.end()?nullptr:i->second.get();}
std::vector<nlohmann::json> ToolRegistry::gemini_declarations()const{std::vector<nlohmann::json> out;for(const auto&[n,t]:tools_)out.push_back({{"type","function"},{"name",n},{"description",t->description()},{"parameters",t->schema()}});return out;}
std::vector<std::string> ToolRegistry::names()const{std::vector<std::string> out;for(const auto&[n,_]:tools_)out.push_back(n);std::sort(out.begin(),out.end());return out;}
bool Tool::validate(const nlohmann::json& args,std::string& error)const{if(!args.is_object()){error="arguments must be a JSON object";return false;}const auto s=schema();for(const auto& r:s.value("required",nlohmann::json::array())){if(!args.contains(r.get<std::string>())){error="missing required argument: "+r.get<std::string>();return false;}}const auto props=s.value("properties",nlohmann::json::object());for(auto it=args.begin();it!=args.end();++it){if(!props.contains(it.key())){error="unknown argument: "+it.key();return false;}const auto type=props[it.key()].value("type","");if(type=="string"&&!it.value().is_string()){error="argument '"+it.key()+"' must be a string";return false;}if(type=="boolean"&&!it.value().is_boolean()){error="argument '"+it.key()+"' must be boolean";return false;}if(type=="integer"&&!it.value().is_number_integer()){error="argument '"+it.key()+"' must be integer";return false;}}return true;}
}
