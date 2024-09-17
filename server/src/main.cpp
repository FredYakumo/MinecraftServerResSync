#include "spdlog/common.h"
#include <cstdint>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <memory>
#include <spdlog/spdlog.h>
#include <boost/beast.hpp>
#include <boost/program_options.hpp>
#include <shared_mutex>
#include "manage_res.h"
#include "config.h"
#include "res_manage.h"

using spdlog::info;
using spdlog::debug;
using spdlog::error;

auto global_managed_res_mtx = std::make_shared<std::shared_mutex>();
auto global_managed_res_data = std::make_shared<models::ManageClassPathMap>();

void init_manage_res_info(const char *config_path) {
    models::ManageClassPathMap manage_res {};
    load_config_from_yaml_file("config.yaml", manage_res);
    std::unique_lock lock(*global_managed_res_mtx);
    *global_managed_res_data = std::move(manage_res);
    // lock.unlock();
}

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::debug);

    info("Start Server");
    
    std::shared_lock lock(*global_managed_res_mtx);
    const auto &managed_res = global_managed_res_data;
    lock.unlock();
    
}