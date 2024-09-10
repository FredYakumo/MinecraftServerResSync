#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <iostream>
#include <sstream>

namespace utils {
    std::string read_to_end(std::ifstream &fs) {
        std::stringstream ss {};
        while (ss << fs.rdbuf()) {}
        return ss.str();
    }
}


#endif