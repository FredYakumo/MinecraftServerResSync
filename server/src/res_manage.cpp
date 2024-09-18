#include "res_manage.h"
#include <boost/filesystem.hpp>
#include <unordered_map>
#include <string>
#include <spdlog/spdlog.h>
#include <fstream>
#include <vector>
#include "utils.hpp"

namespace fs = boost::filesystem;
using spdlog::debug;

std::unordered_map<std::string, std::string> res_manage::get_path_file_list(const char *base_path) {
    fs::path bp = base_path;

    if (!fs::exists(base_path) || !fs::is_directory(base_path)) {
        return std::move(std::unordered_map<std::string, std::string>{});
    }

    std::unordered_map<std::string, std::string> ret {};
    for (const auto &entry : fs::directory_iterator(bp)) {
        if (entry.is_directory()) {
            const auto &p = entry.path().string();
            debug("Look up file path: {0}", p);
            auto path_list = get_path_file_list(p.c_str());
            for (const auto &it : path_list) {
                ret.insert({std::move(it.first), std::move(it.second)});
            }
        }
        else {
            const auto &p = entry.path().string();
            debug("Calculate file's sha1: {0}", p);
            std::ifstream f{p};
            auto sha1 = utils::calculate_str_sha1(utils::read_to_end(f).c_str());
            ret.insert(std::make_pair(p, std::move(sha1)));
        }
    }

    return std::move(ret);
}

std::unordered_map<std::string, std::string> res_manage::fetch_file_hash_map_from_managed_res(const models::ManageClassPathMap &managed_res,
    const std::unordered_set<std::string_view> &skip_class_name_set) {
    for (const auto &it : managed_res.managed_class_map) {
        if (skip_class_name_set.find(it.first) != std::cend(skip_class_name_set)) {
            continue;
        }
        std::vector>
        for (const auto &path : it.second.path_list) {

        }
    }
}