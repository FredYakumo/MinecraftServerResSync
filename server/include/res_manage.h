#ifndef RES_MANAGE_H
#define RES_MANAGE_H

#include <string>
#include <unordered_map>

namespace res_manage {
    std::unordered_map<std::string, std::string> get_path_file_list(const char *base_path);
}

#endif