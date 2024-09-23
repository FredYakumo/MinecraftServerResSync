#include "config.h"
#include "main.h"
#include "models.h"
#include "result.h"
#include "utils.hpp"
#include <fstream>
#include <memory>
#include <mutex>
#include <spdlog/spdlog.h>
#include <string>
#include <yaml-cpp/exceptions.h>
#include <yaml-cpp/yaml.h>

result::Result init_class_file_path(const YAML::Node &node) {
  g_manage_class_path_map.set(std::make_shared<models::ManageClassPathMap>());

  for (auto class_it : node) {
    auto class_name{class_it["class_name"].as<std::string>()};
    spdlog::debug("Class name: {0}", class_name);
    auto path{class_it["path"]};

    auto cs = models::ClassPathList{};

    std::string single_path{};
    try {
      if (auto ps{path.as<std::string>()}; !ps.empty()) {
        spdlog::debug("path(single): {0}", ps);
        cs.path_list.push_back(std::move(ps));
      } else {
        for (auto path_it : path) {
          auto ps_obj = path_it.as<std::string>();
          spdlog::debug("path: {0}", ps_obj);
          cs.path_list.push_back(std::move(ps_obj));
        }
      }
    } catch ([[maybe_unused]] YAML::Exception& e) {
      for (auto path_it : path) {
        auto ps_obj = path_it.as<std::string>();
        spdlog::debug("path: {0}", ps_obj);
        cs.path_list.push_back(std::move(ps_obj));
      }
    }

    g_manage_class_path_map.get_mut().get()->managed_class_map.insert(
        std::make_pair(std::move(class_name), cs));
  }
  return result::Success;
}

result::Result init_server_data_from_config_yaml_file(const char *file_path) {


  spdlog::debug("Load config file from: {0}", file_path);
  auto fs = std::ifstream{file_path};
  if (!fs.is_open()) {
    spdlog::error("load config from yaml failed: {0} not found.", file_path);
    return result::FileNotFound;
  }

  auto config = YAML::Load(utils::read_to_end(fs));

  if (!config["res"]) {
    spdlog::error("Config load failed: not found [res]");
    return result::Failed;
  }
  if (init_class_file_path(config["res"]) != result::Success) {
    spdlog::error("Config load failed at node: res");
    return result::Failed;
  }

  uint64_t port = !config["port"] ? 25576 : config["port"].as<uint64_t>();
  int thread_count = !config["thread_count"] ? 4 : config["thread_count"].as<int>();
  
    auto host = !config["host"] ? "127.0.0.1" : config["host"].as<std::string>();

  g_server_data.set(std::make_shared<models::ServerData>(nullptr, host, port, thread_count));

  return result::Success;
}
