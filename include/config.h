#ifndef CONFIG_H
#define CONFIG_H

#include "result.h"
#include "manage_res.h"

result::Result load_config_from_yaml_file(const char *file_path, models::ManageResource &out_manage_res);

#endif