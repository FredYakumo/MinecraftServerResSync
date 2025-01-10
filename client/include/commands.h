#ifndef COMMANDS_H
#define COMMANDS_H

#include "tclap/Arg.h"
#include "tclap/CmdLine.h"
#include "tclap/ValueArg.h"
#include <unordered_map>

namespace commands {
    class parse_client_commands {
    public:
        parse_client_commands(int argc, char *argv[]): m_cmd("Minecraft Server Resource sync", ' ', "ALPHA") {
            add_args(m_cmd);

            m_cmd.parse(argc, argv);
        }
    private:
        void add_args(TCLAP::CmdLine &cmd) {
            TCLAP::ValueArg<std::string> config_path_arg {"c", "config", "Specify config.yaml path", false, "config.yaml", "string"};
            cmd.add(config_path_arg);
        }
        TCLAP::CmdLine m_cmd;
    };
}


#endif