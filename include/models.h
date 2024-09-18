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
    std::unordered_map<std::string, const std::string> m_file_hash_map;
public:
    FileHashMap(const std::unordered_map<std::string, const std::string> &hash_map): m_file_hash_map(hash_map) {}
    FileHashMap(const std::unordered_map<std::string, const std::string> &&hash_map): m_file_hash_map(hash_map) {}
    inline const std::unordered_map<std::string, const std::string>& file_hash_map() const {
        return m_file_hash_map;
    }
    inline FileHashMap& insert_file_hash(const std::string_view &file_name, const std::string_view &hash) {
        m_file_hash_map.insert({std::string{file_name}, std::string{hash}});
        return *this;
    }
};

class ClassFileResource {
    std::unordered_map<std::string, FileHashMap> m_class_file_hash_map;
public:
    ClassFileResource(const std::unordered_map<std::string, FileHashMap> class_file_hash_map): m_class_file_hash_map(class_file_hash_map) {}
    inline const std::unordered_map<std::string, FileHashMap>& class_file_hash_map() const {
        return m_class_file_hash_map;
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
