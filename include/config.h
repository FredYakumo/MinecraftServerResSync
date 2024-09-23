#ifndef CONFIG_H
#define CONFIG_H

#include "models.h"
#include "result.h"

result::Result init_server_data_from_config_yaml_file(const char *file_path);

#endif