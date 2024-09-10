#include "spdlog/common.h"
#include <cstdint>
#include <iostream>
#include <spdlog/spdlog.h>

using spdlog::info;
using spdlog::debug;
using spdlog::error;

int main() {
    spdlog::set_level(spdlog::level::debug);

    info("Server");
    std::uint64_t v = 0;
    debug("Debug variable: {0}", v);
}