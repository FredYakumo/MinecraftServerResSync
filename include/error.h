#ifndef ERROR_H
#define ERROR_H

#include <string>

enum class error {
    other,
    file_not_found
};

inline std::string to_string(const error &err) {
    switch (err) {
    case error::file_not_found:
        return "File not found";
    default:
        return "Other";
    }
}


#endif