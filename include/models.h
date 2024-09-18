#ifndef MANAGE_RES_H
#define MANAGE_RES_H

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace models {

struct ClassPathList {
  std::vector<std::string> path_list;
};

class ManageClassPathMap {
public:
  std::unordered_map<std::string, ClassPathList> managed_class_map;
};

class FileHashMap {
    std::unordered_map<std::string, const std::string> file_hash_map;
public:
    FileHashMap(const std::unordered_map<std::string, const std::string> &hash_map): file_hash_map(hash_map) {}
    FileHashMap(const std::unordered_map<std::string, const std::string> &&hash_map): file_hash_map(hash_map) {}
    const std::unordered_map<std::string, const std::string>& get_file_hash_map() const {
        return file_hash_map;
    }
};


class ServerData {
    FileHashMap file_hash_map;
    uint64_t listen_port;
public:
    ServerData(const FileHashMap &file_hash_map, uint64_t listen_port): file_hash_map(file_hash_map), listen_port(listen_port) {}
    ServerData(const FileHashMap &&file_hash_map, uint64_t listen_port): file_hash_map(file_hash_map), listen_port(listen_port) {}
};


} // namespace models

#endif