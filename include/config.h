#ifndef CONFIG_H
#define CONFIG_H

#include "result.h"

result::Result init_server_data_from_config_yaml_file(const char *file_path);
void init_manage_res_hash();

#endif