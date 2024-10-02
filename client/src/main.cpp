#include <iostream>
#include "main.h"


using models::ManageClassPathMap;
using models::ServerData;
using models::ShareMutexData;

ShareMutexData<std::shared_ptr<ManageClassPathMap>> g_manage_class_path_map{
    nullptr};
ShareMutexData<std::shared_ptr<ServerData>> g_server_data{nullptr};

int main() {
    std::cout << "client" << std::endl;
}