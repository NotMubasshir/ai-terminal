cmake_minimum_required(VERSION 3.23)
project(AITerminal VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT WIN32)
    message(FATAL_ERROR "AI Terminal v1.0 targets Windows 10/11 x64.")
endif()

include(FetchContent)
FetchContent_Declare(nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
FetchContent_MakeAvailable(nlohmann_json)

set(AI_TERMINAL_SOURCES
    src/ai/GeminiProvider.cpp
    src/ai/MockProvider.cpp
    src/config/Config.cpp
    src/core/AgentState.cpp
    src/core/App.cpp
    src/core/CommandRouter.cpp
    src/security/AuditLogger.cpp
    src/security/PermissionManager.cpp
    src/terminal/WindowsProcess.cpp
    src/terminal/WindowsTerminal.cpp
    src/tools/ToolDispatcher.cpp
    src/tools/ToolRegistry.cpp
    src/tools/FilesystemTools.cpp
    src/tools/CommandTool.cpp
    src/tools/EnvironmentTools.cpp
    src/tools/ProcessTools.cpp
    src/tools/ProjectTools.cpp
    src/tools/GitTools.cpp
)

add_library(ai_terminal_core ${AI_TERMINAL_SOURCES})
target_include_directories(ai_terminal_core PUBLIC include)
target_link_libraries(ai_terminal_core PUBLIC nlohmann_json::nlohmann_json)
target_compile_definitions(ai_terminal_core PRIVATE UNICODE _UNICODE NOMINMAX WIN32_LEAN_AND_MEAN)
target_compile_options(ai_terminal_core PRIVATE /W4 /permissive- /EHsc /utf-8)
target_link_libraries(ai_terminal_core PRIVATE winhttp.lib version.lib)

add_executable(ai-terminal src/main.cpp)
target_link_libraries(ai-terminal PRIVATE ai_terminal_core)
target_compile_definitions(ai-terminal PRIVATE UNICODE _UNICODE NOMINMAX WIN32_LEAN_AND_MEAN)
target_compile_options(ai-terminal PRIVATE /W4 /permissive- /EHsc /utf-8)
set_target_properties(ai-terminal PROPERTIES OUTPUT_NAME "AI-Terminal")

include(CTest)
if(BUILD_TESTING)
    add_executable(ai-terminal-tests
        tests/TestMain.cpp
        tests/ConfigTests.cpp
        tests/PermissionTests.cpp
        tests/ToolRegistryTests.cpp
        tests/StateTests.cpp
        tests/ValidatorTests.cpp
    )
    target_link_libraries(ai-terminal-tests PRIVATE ai_terminal_core)
    target_include_directories(ai-terminal-tests PRIVATE include)
    target_compile_definitions(ai-terminal-tests PRIVATE UNICODE _UNICODE NOMINMAX WIN32_LEAN_AND_MEAN)
    target_compile_options(ai-terminal-tests PRIVATE /W4 /permissive- /EHsc /utf-8)
    add_test(NAME ai-terminal-tests COMMAND ai-terminal-tests)
endif()

install(TARGETS ai-terminal RUNTIME DESTINATION .)
install(DIRECTORY docs DESTINATION .)
install(FILES README.md LICENSE CHANGELOG.md DESTINATION .)

set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_NAME "AI-Terminal")
set(CPACK_PACKAGE_VENDOR "AI Terminal Contributors")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Security-first AI coding and automation terminal for Windows")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_FILE_NAME "AI-Terminal-${PROJECT_VERSION}-Windows-x64")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
include(CPack)
