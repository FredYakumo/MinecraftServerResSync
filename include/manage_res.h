#ifndef MANAGE_RES_H
#define MANAGE_RES_H

#include <string>
#include <string_view>
#include <unordered_map>

namespace models {

class ModelFileClass {
public:
  std::unordered_map<std::string, std::string> file_name_to_hash;
};

class ManageResource {
public:
  ManageResource() : ManageResource("") {}
  ManageResource(const char *class_name) : class_name(class_name) {}

  std::string class_name;
  std::unordered_map<std::string_view, ModelFileClass> managed_class_map;
};

} // namespace models

#endif