#include "manage_res.h"
#include "result.h"
#include "config.h"
#include "utils.hpp"
#include <fstream>
#include <memory>
#include <mutex>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

extern std::shared_ptr<std::mutex> global_managed_res_mtx;
extern std::shared_ptr<models::ManageResource> global_managed_res_data;

result::Result load_config_from_yaml_file(const char *file_path) {
    auto fs = std::ifstream { file_path };
    if (!fs.is_open()) {
        spdlog::error("load config from yaml failed: {0} not found.", file_path);
        return result::FileNotFound;
    }

    auto config = YAML::Load(utils::read_to_end(fs));

    if (!config["res"]) {
        spdlog::error("Config load failed: not found [res]");
        return result::Failed;
    }

    for (auto it : config["res"]) {
        spdlog::info("class_name: {0}\npath: {1}", it["class_name"].as<std::string>(), it["path"].as<std::string>());
    }

    return result::Success;
}