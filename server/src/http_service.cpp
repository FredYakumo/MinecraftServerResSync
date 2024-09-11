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
using string_body = boost::beast::http::basic_string_body<char>;

class Session : public std::enable_shared_from_this<Session> {
    tcp::socket m_socket;
    beast::flat_buffer buffer; // Buffer for reading the request
    http::request<string_body> req;

public:
    explicit Session(tcp::socket socket): m_socket(std::move(socket)) {}
};


result::Result http_service::start_service(const char *host_address, uint16_t port) {
    const auto address = net::ip::make_address(host_address);
    net::io_context ioc {1};

    // std::make_shared<listener>(Types &&Args...)

    return result::Success;
}