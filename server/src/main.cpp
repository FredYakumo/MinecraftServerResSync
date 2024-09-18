#include "spdlog/common.h"
#include <cstdint>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <memory>
#include <spdlog/spdlog.h>
#include <boost/beast.hpp>
#include <boost/program_options.hpp>
#include <shared_mutex>
#include <string_view>
#include <unordered_map>
#include "models.h"
#include "config.h"
#include "res_manage.h"

using spdlog::info;
using spdlog::debug;
using spdlog::error;


models::ShareMutexData<models::ManageClassPathMap> g_manage_class_path_map;
models::ShareMutexData<models::ServerData> g_server_data;

void process_config_file(const char *config_path) {
    models::ManageClassPathMap manage_res {};
    load_config_from_yaml_file(config_path, manage_res);
    g_manage_class_path_map.set(manage_res);
    g_server_data.set(models::ServerData {
        Cla
    })
    // lock.unlock();
}

void init_manage_res_hash(const models::ManageClassPathMap &managed_class_path_map) {
    auto class_file_hash = res_manage::fetch_file_hash_map_from_managed_res(managed_class_path_map, {"unused"});
    g_server_data.set(models::ServerData {})
}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::debug);

    info("Start Server");
    process_config_file("config.yaml");
    init_manage_res_hash(*g_manage_class_path_map.get());
}