#include "spdlog/common.h"
#include <cstdint>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <spdlog/spdlog.h>
#include <boost/beast.hpp>
#include <boost/program_options.hpp>
#include "manage_res.h"
#include "config.h"

using spdlog::info;
using spdlog::debug;
using spdlog::error;

auto global_managed_res_mtx = std::make_shared<std::mutex>();
auto global_managed_res_data = std::make_shared<models::ManageResource>();

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::debug);

    info("Start Server");
    
    load_config_from_yaml_file("config.yaml");
}