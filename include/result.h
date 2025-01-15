#ifndef RESULT_H
#define RESULT_H

#include <error.h>
#include <expected>

namespace wrapper {
    template <typename T> using result = std::expected<T, error>;

    using ok = result<void>;
    template <typename ErrTy> using err = std::unexpected<ErrTy>;
} // namespace wrapper

// #define OK wrapper::result<void>

#endif