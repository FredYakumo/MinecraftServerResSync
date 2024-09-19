#include "res_manage.h"
#include <boost/filesystem.hpp>
#include <unordered_map>
#include <string>
#include <spdlog/spdlog.h>
#include <fstream>
#include <vector>
#include "models.h"
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
            for (const auto & [fst, snd] : path_list) {
                ret.insert({fst, snd});
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

    return ret;
}

models::ClassFileResource res_manage::fetch_file_hash_map_from_managed_res(const models::ManageClassPathMap &managed_res,
    const std::unordered_set<std::string_view> &skip_class_name_set) {
    models::ClassFileResource ret;
    for (const auto &it : managed_res.managed_class_map) {
        if (skip_class_name_set.find(it.first) != std::cend(skip_class_name_set)) {
            continue;
        }
        models::FileHashMap file_hash_map;
        for (const auto &path : it.second.path_list) {
            decltype(auto) file_hash_list = get_path_file_list(path.c_str());
            for (const auto &f_it : file_hash_list) {
                file_hash_map.insert_file_hash(f_it.first, f_it.second);
            }
        }
        ret.insert_file_hash_map(it.first, file_hash_map);
    }
    return ret;
}