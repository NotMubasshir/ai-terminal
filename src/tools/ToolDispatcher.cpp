// SPDX-License-Identifier: Apache-2.0
#include "tools/ToolDispatcher.hpp"
#include <exception>
namespace ai_terminal::tools {
ToolDispatcher::ToolDispatcher(ToolRegistry&r,security::PermissionManager&p,security::AuditLogger&a,const config::AgentLimits&l):registry_(r),permissions_(p),audit_(a),limits_(l){}
std::string ToolDispatcher::bound(const std::string&s,size_t m,bool&t){t=s.size()>m;if(t)return s.substr(0,m)+"\n[TRUNCATED]";return s;}
DispatchResult ToolDispatcher::dispatch(const std::string& name,const nlohmann::json& args,const std::string& reason){
    if(cancelled_)return {false,false,true,"Execution cancelled."};
    if(++calls_>static_cast<size_t>(limits_.max_tool_calls))return {false,false,false,"Tool-call limit reached."};
    auto* tool=registry_.find(name);if(!tool)return {false,true,false,"Unknown tool."};
    std::string error;if(!tool->validate(args,error))return {false,true,false,"Invalid arguments: "+error};
    const auto risk=permissions_.classify(name,args); if(name=="write_file"||name=="create_file"||name=="apply_patch"||name=="delete_file"||name=="move_file"||name=="copy_file") { if(++files_changed_>static_cast<size_t>(limits_.max_files_changed)) return {false,true,false,"File-change limit reached."}; } audit_.tool_request(name,args,risk);
    security::PermissionRequest req{name,risk,args,reason,""};
    auto decision=permissions_.authorize(req);audit_.permission(req,decision);
    if(decision==security::Decision::Deny||decision==security::Decision::AlwaysDeny)return {false,true,false,"Permission denied."};
    if(cancelled_)return {false,false,true,"Execution cancelled."};
    ToolResult result; try { result=tool->execute(args); } catch(const std::exception& ex) { result.ok=false; result.output=std::string("Tool execution failed: ")+ex.what(); } catch(...) { result.ok=false; result.output="Tool execution failed: unknown exception."; } bool trunc=false;auto output=bound(result.output,limits_.max_tool_output_bytes,trunc);audit_.execution(name,result.ok,output,result.exit_code);return {result.ok,false,false,output};
}
}
