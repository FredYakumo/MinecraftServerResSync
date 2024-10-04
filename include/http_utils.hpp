#ifndef HTTP_UTIL_H
#define HTTP_UTIL_H

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/http/verb.hpp>
#include <string_view>
#include <vector>

namespace http_util {
    using byte_type = char;

    std::vector<byte_type> send_request(const std::string_view host, const boost::beast::http::verb method, const std::string &content) {
        boost::beast::net::io_context ioc;
        boost::asio::ip::tcp::resolver resolver{ioc};
        boost::beast::tcp_stream stream{ioc};

        std::vector<byte_type> ret{};
        return ret;
    }
} // namespace http_util

#endif
