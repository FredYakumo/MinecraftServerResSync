#include "main.h"
#include "http_utils.hpp"
#include <boost/beast/http/verb.hpp>
#include <iostream>
#include <spdlog/spdlog.h>
#include <expected>
// #include <tclap/

#include "config.h"

#include "http_utils.hpp"
#include "utils.hpp"

using spdlog::debug;
using spdlog::error;
using spdlog::info;

using models::ManageClassPathMap;
using models::ServerData;
using models::ShareMutexData;

ShareMutexData<std::shared_ptr<ManageClassPathMap>> g_manage_class_path_map{nullptr};
ShareMutexData<std::shared_ptr<ServerData>> g_server_data{nullptr};

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::debug);

    init_server_data_from_config_yaml_file("config.yaml");
    init_manage_res_hash();

    debug("Target server host: {}", g_server_data.get_const()->host());

    auto response = http_util::send_request("127.0.0.1", "/getResourceList", boost::beast::http::verb::get, "");
    std::string content{response.data()};
    info("Send http Response: {}", content);


    // auto command = utils::parse_args(argc, argv);
    // if (command.has_value()) {
        
    // }
}
