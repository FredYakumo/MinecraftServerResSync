#include "main.h"
#include "config.h"
#include "models.h"
#include "res_manage.h"
#include "spdlog/common.h"
#include <boost/beast.hpp>
#include <cstdint>
#include <iostream>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <spdlog/spdlog.h>
#include <string_view>
#include <unordered_map>

using spdlog::debug;
using spdlog::error;
using spdlog::info;

using models::ManageClassPathMap;
using models::ServerData;
using models::ShareMutexData;

ShareMutexData<std::shared_ptr<ManageClassPathMap>> g_manage_class_path_map{
    nullptr};
ShareMutexData<std::shared_ptr<ServerData>> g_server_data{nullptr};

void init_manage_res_hash() {
  const auto class_file_hash = res_manage::fetch_file_hash_map_from_managed_res(
      *g_manage_class_path_map.get_const(), {"unused"});
  g_server_data.get_mut().get()->set_class_file_resources(class_file_hash);
}

int main(int argc, char *argv[]) {
  spdlog::set_level(spdlog::level::debug);

  info("Start Server");
  init_server_data_from_config_yaml_file("config.yaml");
  init_manage_res_hash();
}