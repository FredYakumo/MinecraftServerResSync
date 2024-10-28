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

        std::vector<byte_type> send_request(const std::string_view host, const boost::beast::http::verb method, const std::string &content) {
        boost::beast::net::io_context ioc;
        boost::asio::ip::tcp::resolver resolver{ioc};
        boost::beast::tcp_stream stream{ioc};

        // resolve host address
        auto const results = resolver.resolve(host, "http");
        boost::asio::connect(stream.socket(), results.begin(), results.end());

        // prepare request
        boost::beast::http::request<boost::beast::http::string_body> req{method, "/", content, 11};
        req.set(boost::beast::http::field::host, host);
        req.set(boost::beast::http::field::user_agent, "Beast");
        req.body() = content;
        req.prepare_payload();

        // send request
        boost::beast::http::write(stream, req);

        // recv response
        boost::beast::flat_buffer buffer;
        boost::beast::http::response<boost::beast::http::string_body> res;
        boost::beast::http::read(stream, buffer, res);

        // 
        boost::beast::error_code ec;
        stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

        // check error
        if (ec && ec != boost::beast::errc::not_connected) {
            throw boost::beast::system_error{ec};
        }

        // return response
        return std::vector<byte_type>(res.body().data(), res.body().data() + res.body().size());
    }
} // namespace http_util

#endif
