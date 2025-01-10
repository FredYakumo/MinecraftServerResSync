#ifndef RESULT_H
#define RESULT_H

#include <error.h>
#include <expected>

namespace wrapper {
    template <typename T>
    using result = std::expected<T, error>;
}


#endif