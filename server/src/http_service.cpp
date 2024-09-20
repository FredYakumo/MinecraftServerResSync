#include "http_service.h"
#include "result.h"
#include <boost/beast/http/string_body.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <memory>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
using string_body = http::basic_string_body<char>;

class Session : public std::enable_shared_from_this<Session> {
    tcp::socket m_socket;
    beast::flat_buffer buffer;
    http::request<string_body> req;

public:
    explicit Session(tcp::socket socket): m_socket(std::move(socket)) {}

    void start() {
        read_request();
    }

private:
    void read_request() {

        auto self = shared_from_this();
        http::async_read(m_socket, buffer, req,
            [self](beast::error_code ec, std::size_t bytes_transferred) {
                boost::ignore_unused(bytes_transferred);
                if (!ec) {
                    self->handle_request();
                }
            });
    }

    void handle_request() {

        http::response<string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, "Boost.Beast");
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "Hello, World!";
        res.prepare_payload();


        auto self = shared_from_this();
        http::async_write(m_socket, res,
            [self](beast::error_code ec, std::size_t bytes_transferred) {
                boost::ignore_unused(bytes_transferred);
                if (!ec) {

                    self->m_socket.shutdown(tcp::socket::shutdown_send, ec);
                }
            });
    }
};

// Listener class to accept incoming connections
class Listener : public std::enable_shared_from_this<Listener> {
    tcp::acceptor acceptor_;
    tcp::socket socket_;

public:
    Listener(net::io_context& ioc, tcp::endpoint endpoint)
        : acceptor_(ioc), socket_(ioc) {

        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(net::socket_base::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen(net::socket_base::max_listen_connections);


        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(socket_,
            [self = shared_from_this()](beast::error_code ec) {
                if (!ec) {

                    std::make_shared<Session>(std::move(self->socket_))->start();
                }


                self->do_accept();
            });
    }
};



result::Result http_service::start_service(const char *host_address, uint16_t port) {
    try {
        const auto address = net::ip::make_address(host_address);
        net::io_context ioc{1};


        tcp::endpoint endpoint{address, port};
        std::make_shared<Listener>(ioc, endpoint);


        ioc.run();

        return result::Success;
    } catch (const std::exception& e) {
        return result::Failed;
    }
}