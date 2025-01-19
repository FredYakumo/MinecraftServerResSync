#include "commands.h"
#include "config.h"
#include "main.h"
#include "result.h"
#include <spdlog/spdlog.h>
#include <string>
#include <string_view>
#include <tclap/CmdLine.h>
#include <tclap/ValueArg.h>

namespace cmd_values {
    TCLAP::ValueArg<std::string> config_path_arg{"c", "config_path", "Specify config yaml file path", false, "config.yaml", "string"};

    void add_value_args(TCLAP::CmdLine &cmd) { cmd.add(config_path_arg); }
} // namespace cmd_values

namespace commands {
    using spdlog::error;
    using spdlog::info;
    using wrapper::err;
    using wrapper::ok;

    wrapper::result<void> load_config(const std::string_view config_path) {
        const auto res = init_server_data_from_config_yaml_file(config_path);
        if (!res.has_value()) {
            error("Load config error: {}", to_string(res.error()));
            return err(res.error());
        }
        init_manage_res_hash();
        info("Target server address: {}:{}", g_server_data.get_const()->host(), g_server_data.get_const()->listen_port());

        return ok();
    }

    void run_commands(int argc, char *argv[]) {
        TCLAP::CmdLine cmd{"MinecraftServer Res Sync", ' ', "ALPHA"};

        cmd_values::add_value_args(cmd);

        cmd.parse(argc, argv);

        load_config(cmd_values::config_path_arg.getValue());
    }
} // namespace commands