#ifndef HTTP_SERVICE_H
#define HTTP_SERVICE_H

#include "result.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/status.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/verb.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/format.hpp>
#include <boost/format/format_fwd.hpp>
#include <boost/json.hpp>
#include <boost/json/serialize.hpp>
#include <boost/json/value.hpp>
#include <boost/thread.hpp>
#include <boost/thread/detail/thread.hpp>
#include <cstdint>
#include <memory>
#include <models.h>
#include <result.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace http_service {
    namespace beast = boost::beast; // from <boost/beast.hpp>
    namespace http = beast::http;   // from <boost/beast/http.hpp>
    namespace net = boost::asio;    // from <boost/asio.hpp>
    namespace websocket = beast::websocket;
    using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

    inline std::string thread_id_to_string(const boost::thread::id &thread_id) {
        std::ostringstream oss;
        oss << thread_id;
        return oss.str();
    }

    class Session : public std::enable_shared_from_this<Session> {
    public:
        explicit
        Session(tcp::socket socket,
                const std::unordered_map<std::string, models::Api> &bind_apis)
            : m_socket(std::move(socket)), m_bind_apis(bind_apis) {}

        void start() { do_read(); }

    private:
        void do_read();

        void handle_request();

        void do_write(http::response<http::string_body> &&res);

        tcp::socket m_socket;
        beast::flat_buffer m_buffer;
        http::request<http::string_body> m_req;
        const std::unordered_map<std::string, models::Api> &m_bind_apis;
    };

    class WebSocketSession
        : public std::enable_shared_from_this<WebSocketSession> {
    public:
        explicit WebSocketSession(tcp::socket socket, net::io_context &io_context)
            : m_ws(std::move(socket)), m_timer(io_context) {}

        void start() { do_accept(); }

    private:
        void do_accept() {
            m_ws.async_accept(std::bind(&WebSocketSession::on_accept,
                                        shared_from_this(),
                                        std::placeholders::_1));

        }

        void on_accept(beast::error_code ec) {
            if (ec) {
                spdlog::error("Error on accept: {}", ec.message());
                return;
            }
            // send_message("Hello world");
            // do_read();
            send_number();
        }

        void do_read() {
            m_ws.async_read(m_buffer, [self = shared_from_this()](
                                          beast::error_code ec,
                                          std::size_t bytes_transferred) {
                self->on_read(ec, bytes_transferred);
            });
        }

        void on_read(const beast::error_code &ec,
                     std::size_t bytes_transferred) {
            if (ec) {
                spdlog::error("Error on read: {}", ec.message());
                return;
            }

            m_ws.async_write(
                m_buffer.data(),
                std::bind(&WebSocketSession::on_write, shared_from_this(),
                          std::placeholders::_1, std::placeholders::_2));
        }

        void on_write(const beast::error_code &ec,
                      std::size_t /*bytes_transferred*/) {
            if (ec) {
                spdlog::error("Error on write: {}", ec.message());
                return;
            }
            m_buffer.clear();
            do_read();
        }

        void send_number();

        void send_message(const std::string &message) {
            m_ws.async_write(
                net::buffer(message), [](const beast::error_code &ec,
                                         std::size_t /*bytes_transferred*/) {
                    if (ec)
                        spdlog::error("Error on sending message: {}",
                                      ec.message());
                });
        }

        models::ShareMutexData<int> m_counter { 0 };
        websocket::stream<tcp::socket> m_ws;
        beast::flat_buffer m_buffer;
        net::steady_timer m_timer;
        bool m_disconnected = false;
    };

    class HttpServer {
    public:
        HttpServer(const std::string_view address, unsigned short port,
                   const int num_threads)
            : m_ioc(num_threads),
              m_acceptor(m_ioc, {net::ip::make_address(address), port}),
              m_num_threads(num_threads) {}

        HttpServer &register_api(const std::string_view url, models::Api api) {
            m_bind_apis.emplace(std::string{url}, std::move(api));
            return *this;
        }

        void run() {
            do_accept();
            start_threads();
        }

    private:
        void do_accept();

        void start_threads();

        net::io_context m_ioc;    // IO Context
        tcp::acceptor m_acceptor; // acceptor
        std::size_t m_num_threads;
        std::vector<boost::thread> m_threads;
        std::unordered_map<std::string, models::Api> m_bind_apis;
    };

    inline void write_json_result(boost::json::value json,
                                  http::response<http::string_body> &out_res) {
        out_res.result(http::status::ok);
        out_res.set(http::field::content_type, "application/json");
        out_res.body() = serialize(json);

        out_res.prepare_payload();
    }

    result::Result start_service(std::string_view host_address, uint16_t port,
                                 int threads_count);
} // namespace http_service

#endif