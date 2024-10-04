#include "main.h"
#include <iostream>
#include <spdlog/spdlog.h>

#include "config.h"

using spdlog::debug;
using spdlog::error;
using spdlog::info;

using models::ManageClassPathMap;
using models::ServerData;
using models::ShareMutexData;

ShareMutexData<std::shared_ptr<ManageClassPathMap>> g_manage_class_path_map{
    nullptr};
ShareMutexData<std::shared_ptr<ServerData>> g_server_data{nullptr};

int main() {
    spdlog::set_level(spdlog::level::debug);
    
    init_server_data_from_config_yaml_file("config.yaml");
    init_manage_res_hash();
    
    debug("Target server host: {}", g_server_data.get_const()->host());
}
