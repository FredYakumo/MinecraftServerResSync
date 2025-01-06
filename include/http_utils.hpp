#ifndef HTTP_UTIL_H
#define HTTP_UTIL_H

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/verb.hpp>
#include <boost/beast/http/write.hpp>
#include <string_view>
#include <vector>

namespace http_util {
    using byte_type = char;
    namespace beast = boost::beast;
    namespace asio = boost::asio;


    std::vector<byte_type> send_request(const std::string_view host, const beast::http::verb method, const std::string &content) {
        beast::net::io_context ioc;
        asio::ip::tcp::resolver resolver{ioc};
        beast::tcp_stream stream{ioc};

        // resolve host address
        auto const results = resolver.resolve(host, "http");
        asio::connect(stream.socket(), results.begin(), results.end());

        beast::http::request<beast::http::string_body> req;
        req.target(host);
        req.method(method);
        req.set(boost::beast::http::field::host, host);
        req.set(boost::beast::http::field::user_agent, "MinecraftResSyncClient");
        req.body() = content;
        req.prepare_payload();


        boost::beast::http::write(stream, req);


        beast::flat_buffer buffer;
        beast::http::response<boost::beast::http::string_body> res;
        beast::http::read(stream, buffer, res);

        beast::error_code ec;
        stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

        if (ec && ec != boost::beast::errc::not_connected) {
            throw boost::beast::system_error{ec};
        }

        return std::vector<byte_type>(res.body().data(), res.body().data() + res.body().size());
    }
} // namespace http_util

#endif
