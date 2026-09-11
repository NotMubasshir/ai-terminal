// SPDX-License-Identifier: Apache-2.0
#pragma once
#include "tools/Tool.hpp"
#include <filesystem>
#include <memory>
namespace ai_terminal::tools {
std::unique_ptr<Tool> make_get_working_directory_tool(const std::filesystem::path& root);
std::unique_ptr<Tool> make_set_working_directory_tool(const std::filesystem::path& root,std::function<bool(const std::filesystem::path&)> setter);
std::unique_ptr<Tool> make_list_directory_tool(const std::filesystem::path& root);
std::unique_ptr<Tool> make_read_file_tool(const std::filesystem::path& root,size_t max_read);
std::unique_ptr<Tool> make_create_file_tool(const std::filesystem::path& root,size_t max_write);
std::unique_ptr<Tool> make_write_file_tool(const std::filesystem::path& root,size_t max_write);
std::unique_ptr<Tool> make_create_directory_tool(const std::filesystem::path& root);
std::unique_ptr<Tool> make_delete_file_tool(const std::filesystem::path& root);
std::unique_ptr<Tool> make_copy_file_tool(const std::filesystem::path& root);
std::unique_ptr<Tool> make_move_file_tool(const std::filesystem::path& root);
std::unique_ptr<Tool> make_search_files_tool(const std::filesystem::path& root,size_t max_output);
std::unique_ptr<Tool> make_file_exists_tool(const std::filesystem::path& root);
std::unique_ptr<Tool> make_file_info_tool(const std::filesystem::path& root);
std::unique_ptr<Tool> make_apply_patch_tool(const std::filesystem::path& root,size_t max_write);
}
