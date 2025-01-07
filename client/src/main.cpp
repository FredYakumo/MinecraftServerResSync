#include "main.h"
#include "http_utils.hpp"
#include <boost/beast/http/verb.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

#include "config.h"

#include "http_utils.hpp"

using spdlog::debug;
using spdlog::error;
using spdlog::info;

using models::ManageClassPathMap;
using models::ServerData;
using models::ShareMutexData;

ShareMutexData<std::shared_ptr<ManageClassPathMap>> g_manage_class_path_map{nullptr};
ShareMutexData<std::shared_ptr<ServerData>> g_server_data{nullptr};

int main() {
    spdlog::set_level(spdlog::level::debug);

    init_server_data_from_config_yaml_file("config.yaml");
    init_manage_res_hash();

    debug("Target server host: {}", g_server_data.get_const()->host());

    auto response = http_util::send_request("127.0.0.1", "/getResourceList", boost::beast::http::verb::get, "");
    // debug("response: {}", std::string{ std::cbegin(response), std::cend(response)});
    std::string content{response.data()};
    info("Send http Response: {}", content);
}
