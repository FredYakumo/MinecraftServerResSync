//
// Created by fredyakumo on 9/20/2024.
//

#ifndef MAIN_H
#define MAIN_H

#include "models.h"

extern models::ShareMutexData<std::shared_ptr<models::ManageClassPathMap>> g_manage_class_path_map;
extern models::ShareMutexData<std::shared_ptr<models::ServerData>> g_server_data;

#endif //MAIN_H
