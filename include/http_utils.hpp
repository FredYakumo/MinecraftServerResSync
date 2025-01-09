#ifndef HTTP_UTIL_H
#define HTTP_UTIL_H

#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/core/error.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/dynamic_body.hpp>
#include <boost/beast/http/field.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/system/detail/error_code.hpp>
#include <exception>
#include <string>
#include <string_view>
#include <vector>
#include <spdlog/spdlog.h>

namespace http_util {
    using byte_type = char;
    namespace beast = boost::beast;
    namespace http = beast::http;
    namespace asio = boost::asio;

    std::vector<byte_type> send_request(const std::string_view host, const std::string_view path, const http::verb method, const std::string &content) {
        try {
            asio::io_context ioc;

            asio::ip::tcp::resolver resolver{ioc};
            auto const results = resolver.resolve(host, "23333");

            asio::ip::tcp::socket socket{ioc};
            asio::connect(socket, results.cbegin(), results.cend());

            http::request<http::string_body> req{method, path, 11};
            req.set(http::field::host, host);

            http::write(socket, req);

            beast::flat_buffer buffer;

            std::vector<byte_type> res;
            http::response<http::dynamic_body> response_body;

            http::read(socket, buffer, response_body);

            auto &body = response_body.body();
            const auto &buffers = body.data();
            for (const auto &buffer : buffers) {
                const byte_type *data = asio::buffer_cast<const byte_type *>(buffer);
                for (size_t i = 0; i < buffer.size(); ++i) {
                    res.push_back(*(data + i));
                }
            }
            boost::system::error_code ec;
            socket.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
            if (ec && ec != asio::error::not_connected) {
                spdlog::error("{}", ec.what());
                throw beast::system_error{ec};
            }
            return res;
        } catch (const std::exception &e) {
            spdlog::error("Send http request error: {}", e.what());
            throw e;
        }
    }

} // namespace http_util

#endif
