// SPDX-License-Identifier: Apache-2.0
#include "security/PermissionManager.hpp"
#include <algorithm>
#include <cctype>
namespace ai_terminal::security {
PermissionManager::PermissionManager(ConfirmFn confirm):confirm_(std::move(confirm)){}
void PermissionManager::set_mode(config::PermissionMode m){mode_=m;}
void PermissionManager::set_safe_mode(bool e){safe_mode_=e;}
void PermissionManager::reset_session(){session_allowed_.clear();}
RiskLevel PermissionManager::classify(const std::string& n,const nlohmann::json& a) const {
    if(n=="format_disk"||n=="disable_security"||n=="extract_credentials") return RiskLevel::BLOCKED;
    if(n=="get_working_directory"||n=="list_directory"||n=="file_exists"||n=="get_file_info"||n=="detect_project_type"||n=="inspect_project"||n=="git_status"||n=="git_diff"||n=="git_log"||n=="git_branch"||n=="get_environment"||n=="get_system_info"||n=="list_processes") return RiskLevel::SAFE;
    if(n=="read_file"||n=="search_files"||n=="create_directory"||n=="create_file"||n=="get_process_info") return RiskLevel::LOW;
    if(n=="execute_command"||n=="execute_powershell"){auto c=a.value("command",std::string{});std::string l=c;std::transform(l.begin(),l.end(),l.begin(),[](unsigned char x){return static_cast<char>(std::tolower(x));});for(const auto&needle:{"format ","diskpart","shutdown ","bcdedit","reg delete","sc delete","vssadmin delete","net user","takeown ","remove-item -recurse","set-executionpolicy","stop-computer","clear-disk","cipher /w","del /s /q"})if(l.find(needle)!=std::string::npos)return RiskLevel::HIGH;return RiskLevel::MEDIUM;}
    if(n=="build_project"||n=="test_project") return RiskLevel::MEDIUM;
    if(n=="write_file"||n=="apply_patch"||n=="copy_file"||n=="move_file"||n=="set_working_directory") return RiskLevel::MEDIUM;
    if(n=="delete_file"||n=="git_add"||n=="git_commit"||n=="git_checkout"||n=="terminate_process") return RiskLevel::HIGH;
    if(n.rfind("git_",0)==0) return RiskLevel::HIGH;
    (void)a; return RiskLevel::BLOCKED;
}
Decision PermissionManager::authorize(const PermissionRequest& r){
    if(r.risk==RiskLevel::BLOCKED||always_denied_.count(r.tool_name)) return Decision::Deny;
    if(safe_mode_ && r.risk>=RiskLevel::MEDIUM) return Decision::Deny;
    if(mode_==config::PermissionMode::AlwaysDeny) return Decision::Deny;
    if(session_allowed_.count(r.tool_name)) return Decision::AllowSession;
    if(mode_==config::PermissionMode::SafeAuto && r.risk<=RiskLevel::LOW) return Decision::AllowOnce;
    if(mode_==config::PermissionMode::DangerousOnly && r.risk<=RiskLevel::MEDIUM) return Decision::AllowOnce;
    if(mode_==config::PermissionMode::Session && r.risk<=RiskLevel::MEDIUM){session_allowed_.insert(r.tool_name);return Decision::AllowSession;}
    if(!confirm_) return Decision::Deny;
    const auto d=confirm_(r);
    if(d==Decision::AllowSession) session_allowed_.insert(r.tool_name);
    if(d==Decision::AlwaysDeny) always_denied_.insert(r.tool_name);
    return d;
}
bool PermissionManager::is_session_allowed(const std::string& n) const{return session_allowed_.count(n)!=0;}
const char* risk_name(RiskLevel r){switch(r){case RiskLevel::SAFE:return "SAFE";case RiskLevel::LOW:return "LOW";case RiskLevel::MEDIUM:return "MEDIUM";case RiskLevel::HIGH:return "HIGH";case RiskLevel::CRITICAL:return "CRITICAL";case RiskLevel::BLOCKED:return "BLOCKED";}return "UNKNOWN";}
const char* decision_name(Decision d){switch(d){case Decision::Deny:return "DENY";case Decision::AllowOnce:return "ALLOW_ONCE";case Decision::AllowSession:return "ALLOW_SESSION";case Decision::AlwaysDeny:return "ALWAYS_DENY";}return "UNKNOWN";}
}
