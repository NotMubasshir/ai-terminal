// SPDX-License-Identifier: Apache-2.0
#include "security/AuditLogger.hpp"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
namespace ai_terminal::security {
AuditLogger::AuditLogger(std::filesystem::path p,size_t m,size_t o):path_(std::move(p)),max_bytes_(m),max_output_bytes_(o){std::filesystem::create_directories(path_.parent_path());}
std::string AuditLogger::timestamp(){auto now=std::chrono::system_clock::now();auto t=std::chrono::system_clock::to_time_t(now);std::tm tm{};localtime_s(&tm,&t);std::ostringstream s;s<<std::put_time(&tm,"%Y-%m-%dT%H:%M:%S");return s.str();}
nlohmann::json AuditLogger::redact(nlohmann::json v){
    if(v.is_object()){for(auto it=v.begin();it!=v.end();++it){const auto k=it.key();if(k.find("key")!=std::string::npos||k.find("token")!=std::string::npos||k.find("password")!=std::string::npos||k.find("secret")!=std::string::npos||k.find("credential")!=std::string::npos) it.value()="[REDACTED]"; else it.value()=redact(it.value());}}
    else if(v.is_array()) for(auto& x:v) x=redact(x);
    else if(v.is_string()){auto s=v.get<std::string>();if(s.find("AIza")!=std::string::npos)v="[REDACTED]";}
    return v;
}
void AuditLogger::rotate_if_needed(){std::error_code ec;auto sz=std::filesystem::exists(path_,ec)?std::filesystem::file_size(path_,ec):0;if(ec||sz<max_bytes_)return;auto old=path_;old+=L".1";std::filesystem::remove(old,ec);std::filesystem::rename(path_,old,ec);}
void AuditLogger::event(const std::string& name,const nlohmann::json& data){std::lock_guard lock(mutex_);rotate_if_needed();std::ofstream out(path_,std::ios::app);if(!out)return;out<<"{"<<"\"timestamp\":"<<nlohmann::json(timestamp()).dump()<<",\"event\":"<<nlohmann::json(name).dump()<<",\"data\":"<<redact(data).dump()<<"}\n";}
void AuditLogger::tool_request(const std::string& tool,const nlohmann::json& args,RiskLevel risk){event("TOOL_REQUEST",{{"tool",tool},{"arguments",args},{"risk",risk_name(risk)}});}
void AuditLogger::permission(const PermissionRequest& r,Decision d){event("PERMISSION",{{"tool",r.tool_name},{"risk",risk_name(r.risk)},{"decision",decision_name(d)}});}
void AuditLogger::execution(const std::string& tool,bool ok,const std::string& output,long code){std::string bounded=output;if(bounded.size()>max_output_bytes_)bounded.resize(max_output_bytes_);event("EXECUTION",{{"tool",tool},{"success",ok},{"exit_code",code},{"output",bounded}});}
}
