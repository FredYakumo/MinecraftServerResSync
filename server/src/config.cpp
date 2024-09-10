#include "manage_res.h"
#include "result.h"
#include "config.h"
#include "manage_res.h"
#include "utils.hpp"
#include <fstream>
#include <memory>
#include <mutex>
#include <spdlog/spdlog.h>
#include <string>
#include <yaml-cpp/yaml.h>

extern std::shared_ptr<std::mutex> global_managed_res_mtx;
extern std::shared_ptr<models::ManageResource> global_managed_res_data;

result::Result load_config_from_yaml_file(const char *file_path, models::ManageResource &out_manage_res) {
    spdlog::debug("Load config file from: {0}", file_path);
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

    out_manage_res = models::ManageResource {};

    for (auto class_it : config["res"]) {
        auto class_name { class_it["class_name"].as<std::string>() };
        spdlog::debug("Class name: {0}", class_name);
        auto path { class_it["path"] };

        auto cs = models::ModelFileClass {};
        
        auto single_path = path.as<std::string>();

        if (single_path.size() > 0) {
            spdlog::debug("path(single): {0}", single_path);
            cs.path_list.push_back(std::move(single_path));
        } else {
            for (auto path_it : path) {
                std::string ps_obj = path_it.as<std::string>();
                spdlog::debug("path: {0}", ps_obj);
                cs.path_list.push_back(std::move(ps_obj));
            }
        }

        out_manage_res.managed_class_map.insert(std::make_pair(std::move(class_name), cs));
    }

    return result::Success;
}