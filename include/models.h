#ifndef MANAGE_RES_H
#define MANAGE_RES_H

#include <algorithm>
#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace models {

template <typename T> class ShareMutexData {
  std::shared_ptr<T> data;
  mutable std::shared_mutex mtx;

public:
  ShareMutexData() : data(nullptr) {}

  template <typename U>
  explicit ShareMutexData(U &&data)
      : data(std::make_shared<T>(std::forward<U>(data))) {}

  inline void set(std::optional<T> d) {
    std::unique_lock lock(mtx);
    if (d) {
      if (!data) {
        data = std::make_shared<T>(std::move(*d));
      } else {
        *data = std::move(*d);
      }
    } else {
      data.reset();
    }
  }

  inline const std::shared_ptr<T> get() const {
    std::shared_mutex lock(mtx);
    return data;
  }

  inline T *unsafeGet() { return data ? data.get() : nullptr; }
};

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
  FileHashMap() = default;

  explicit FileHashMap(
      std::unordered_map<std::string, const std::string> hash_map)
      : m_file_hash_map(std::move(hash_map)) {}

  // FileHashMap(const std::unordered_map<std::string, const std::string>
  // &hash_map): m_file_hash_map(hash_map) {} FileHashMap(const
  // std::unordered_map<std::string, const std::string> &&hash_map):
  // m_file_hash_map(hash_map) {}

  inline const std::unordered_map<std::string, const std::string> &
  file_hash_map() const {
    return m_file_hash_map;
  }
  inline FileHashMap &insert_file_hash(std::string_view file_name,
                                       std::string_view hash) {
    m_file_hash_map.emplace(file_name, hash);
    return *this;
  }
};

class ClassFileResource {
  std::unordered_map<std::string, FileHashMap> m_class_file_hash_map;

public:
  ClassFileResource() = default;
  explicit ClassFileResource(
      std::unordered_map<std::string, FileHashMap> class_file_hash_map)
      : m_class_file_hash_map(std::move(class_file_hash_map)) {}
  // ClassFileResource(const std::unordered_map<std::string, FileHashMap>
  // class_file_hash_map): m_class_file_hash_map(class_file_hash_map) {}
  inline const std::unordered_map<std::string, FileHashMap> &
  class_file_hash_map() const {
    return m_class_file_hash_map;
  }
  inline ClassFileResource &
  insert_file_hash_map(std::string_view class_name,
                       const FileHashMap &file_hash_map) {
    m_class_file_hash_map.insert({std::string{class_name}, file_hash_map});
    return *this;
  }
};

class ServerData {
  ShareMutexData<ClassFileResource> class_file_resources;
  uint64_t listen_port;

public:
  ServerData() = default;
  explicit ServerData(ClassFileResource class_file_resource,
                      uint64_t listen_port)
      : class_file_resources(std::move(class_file_resource)),
        listen_port(std::move(listen_port)) {}
};

} // namespace models

#endif
