#ifndef MANAGE_RES_H
#define MANAGE_RES_H

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace models {

struct ModelFileClass {
  std::vector<std::string> path_list;
};

class ManageResource {
public:
  std::unordered_map<std::string, ModelFileClass> managed_class_map;
};

} // namespace models

#endif