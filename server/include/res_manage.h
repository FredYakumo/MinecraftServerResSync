#ifndef RES_MANAGE_H
#define RES_MANAGE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "models.h"

namespace res_manage {
    std::unordered_map<std::string, std::string> get_path_file_list(const char *base_path);
    models::ClassFileResource fetch_file_hash_map_from_managed_res(const models::ManageClassPathMap &managed_res,
        const std::unordered_set<std::string_view> &skip_class_name_set);
}

#endif