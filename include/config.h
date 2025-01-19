#ifndef CONFIG_H
#define CONFIG_H

#include "result.h"

wrapper::result<void> init_server_data_from_config_yaml_file(const std::string_view file_path);
void init_manage_res_hash();

#endif