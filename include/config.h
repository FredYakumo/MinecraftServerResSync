#ifndef CONFIG_H
#define CONFIG_H

#include "result.h"
#include "models.h"

result::Result init_server_data_from_config_yaml_file(const char *file_path);

#endif