#include "res_manage.h"
#include "models.h"
#include "utils.hpp"
#include <boost/filesystem.hpp>
#include <fstream>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace fs = boost::filesystem;
using spdlog::debug;

std::unordered_map<std::string, std::string>
res_manage::get_path_file_list(const char *base_path) {
    fs::path bp = base_path;

    if (!fs::exists(base_path) || !fs::is_directory(base_path)) {
        return std::move(std::unordered_map<std::string, std::string>{});
    }

    std::unordered_map<std::string, std::string> ret{};
    for (const auto &entry : fs::directory_iterator(bp)) {
        if (entry.is_directory()) {
            const auto &p = entry.path().string();
            debug("Look up file path: {0}", p);
            for (auto path_list = get_path_file_list(p.c_str());
                 const auto &[fst, snd] : path_list) {
                ret.insert({fst, snd});
            }
        } else {
            const auto &p = entry.path().string();
            debug("Calculate file's sha1: {0}", p);
            std::ifstream f{p};
            auto sha1 =
                utils::calculate_str_sha1(utils::read_to_end(f).c_str());
            ret.insert(std::make_pair(p, std::move(sha1)));
        }
    }

    return ret;
}

models::ClassFileResource res_manage::fetch_file_hash_map_from_managed_res(
    const models::ManageClassPathMap &managed_res,
    const std::unordered_set<std::string_view> &skip_class_name_set) {
    models::ClassFileResource ret;
    for (const auto &[fst, snd] : managed_res.managed_class_map) {
        if (skip_class_name_set.contains(fst)) {
            continue;
        }
        models::FileHashMap file_hash_map;
        for (const auto &path : snd.path_list) {
            for (decltype(auto) file_hash_list =
                     get_path_file_list(path.c_str());
                 const auto &[fst, snd] : file_hash_list) {
                file_hash_map.insert_file_hash(fst, snd);
            }
        }
        ret.insert_file_hash_map(fst, file_hash_map);
    }
    return ret;
}