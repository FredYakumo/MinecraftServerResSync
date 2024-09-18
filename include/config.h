#ifndef CONFIG_H
#define CONFIG_H

#include "result.h"
#include "models.h"

result::Result load_config_from_yaml_file(const char *file_path, models::ManageClassPathMap &out_manage_res);

#endif