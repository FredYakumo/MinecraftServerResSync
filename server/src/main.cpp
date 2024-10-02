#include "main.h"
#include "config.h"
#include "models.h"
#include "res_manage.h"
#include "spdlog/common.h"
#include <http_service.h>
#include <memory>
#include <spdlog/spdlog.h>
#include <string_view>

using spdlog::debug;
using spdlog::error;
using spdlog::info;

using models::ManageClassPathMap;
using models::ServerData;
using models::ShareMutexData;

ShareMutexData<std::shared_ptr<ManageClassPathMap>> g_manage_class_path_map{
    nullptr};
ShareMutexData<std::shared_ptr<ServerData>> g_server_data{nullptr};

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::debug);

    info("Start Server");
    init_server_data_from_config_yaml_file("config.yaml");
    init_manage_res_hash();

    http_service::start_service(g_server_data.get_const()->host(),
                                g_server_data.get_const()->listen_port(),
                                g_server_data.get_const()->thread_count());
}
