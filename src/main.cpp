// SPDX-License-Identifier: Apache-2.0
#include "core/App.hpp"
#include "config/Config.hpp"
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>
int main(int argc,char**argv){try{for(int i=1;i<argc;++i){std::string a=argv[i];if(a=="--help"){std::cout<<"AI Terminal 1.0.0\nUsage: AI-Terminal [--help] [--version] [--config] [--reset] [--safe-mode]\n";return 0;}if(a=="--version"){std::cout<<"1.0.0\n";return 0;}if(a=="--config"){std::cout<<ai_terminal::config::ConfigStore::config_path().string()<<"\n";return 0;}if(a=="--reset"){std::string e;auto p=ai_terminal::config::ConfigStore::config_path();std::error_code ec;std::filesystem::remove(p,ec);if(ec){std::cerr<<"Unable to reset configuration: "<<ec.message()<<"\n";return 1;}std::cout<<"Configuration reset.\n";return 0;}if(a=="--safe-mode")_putenv_s("AI_TERMINAL_SAFE_MODE","1");}return ai_terminal::core::App{}.run();}catch(const std::exception&e){std::cerr<<"Fatal error: "<<e.what()<<"\n";return 1;}catch(...){std::cerr<<"Fatal error: unknown exception.\n";return 1;}}
