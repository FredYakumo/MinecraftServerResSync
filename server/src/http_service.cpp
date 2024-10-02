#include "http_service.h"
#include "components.h"
#include <boost/asio.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/json.hpp>
#include <boost/url/parse.hpp>
#include <exception>
#include <spdlog/spdlog.h>
#include <utility>
// #define BOOST_URL_NO_LIB
#include <boost/url.hpp>
#include <utils.hpp>

namespace http_service {
    namespace beast = boost::beast;   // from <boost/beast.hpp>
    namespace http = beast::http;     // from <boost/beast/http.hpp>
    using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

    void Session::do_read() {
        auto self = shared_from_this();
        async_read(m_socket, m_buffer, m_req, [self](beast::error_code ec, std::size_t) {
            if (!ec) {
                self->handle_request();
            }
        });
    }

    void Session::handle_request() {
        const std::string_view target{m_req.target()};
        const boost::system::result<boost::url_view> url{boost::urls::parse_uri_reference(target)};

        http::response<http::dynamic_body> res;

        if (!url.has_value()) {
            spdlog::error("Invalid URL: {}", url.error().message());
            res.result(http::status::internal_server_error);
            http_service::write_json_result(http::status::ok, boost::json::value {
                {"message", fmt::format("Invalid URL: {}", target)}
            }, res);
            do_write(std::move(res));
            return;
        }

        spdlog::info("Get request from: {0}, path: {1}", m_socket.remote_endpoint().address().to_string(), target);

        const auto thread_id = boost::this_thread::get_id();
        spdlog::debug("Thread id: {0}", thread_id_to_string(thread_id));

        // route
        if (const auto &api = m_bind_apis.find(url->path()); api != m_bind_apis.cend()) {
            if (m_req.method() != api->second.method) {
                res.result(http::status::bad_request);
                res.set(http::field::content_type, "text/plain");

                ostream(res.body()) << (boost::format("Bad request method: {}") % m_req.method()).str();
                res.prepare_payload();
            } else {
                try {
                    const std::string param{url->query()};
                    const auto query_it = utils::SplitString(param, '&');
                    std::unordered_map<std::string_view, std::string_view> param_map{};
                    for (const auto begin : query_it) {
                        utils::SplitString::Iterator p_it = utils::SplitString(begin, '=').begin();
                        const auto key = *p_it++;
                        const auto value = *p_it;
                        param_map.emplace(key, value);
                    }

                    api->second.api_function(m_req, res, param_map);
                } catch (const std::exception &e) {
                    spdlog::error("{}", e.what());
                }
            }
        } else {
            res.result(http::status::not_found);
            ostream(res.body()) << "404: Not Found";
            res.prepare_payload();
        }

        do_write(std::move(res));
    }

    void Session::do_write(http::response<http::dynamic_body> &&res) {
        auto sp_res = std::make_shared<http::response<http::dynamic_body>>(std::move(res));
        auto self = shared_from_this();
        http::async_write(m_socket, *sp_res, [self, sp_res](beast::error_code ec, std::size_t) {
            if (!ec) {
                self->m_socket.shutdown(tcp::socket::shutdown_send, ec);
            }
        });
    }

    void WebSocketSession::send_number() {
        if (m_disconnected) {
            spdlog::info("Aborted");
            return;
        }
        auto self = shared_from_this();
        m_counter.get_mut().get() += 1;
        std::string d_str = std::to_string(m_counter.get_const());
        const auto mutable_buffers = m_buffer.prepare(d_str.size());
        std::memcpy(mutable_buffers.data(), d_str.data(), d_str.size());
        m_buffer.commit(d_str.size());
        m_ws.async_write(m_buffer.data(), [self](beast::error_code ec, std::size_t /*bytes_transferred*/) {
            if (ec) {
                spdlog::error("Error sending number:  {}", ec.message());
                self->m_disconnected = true;
                self->m_timer.cancel();
            }
        });
        m_buffer.clear();

        spdlog::info("Send number: {}", m_counter.get_const());

        m_timer.expires_after(std::chrono::seconds(1));
        m_timer.async_wait([self](beast::error_code ec) {
            if (!ec) {
                self->send_number();
            }
        });
    }

    void HttpServer::do_accept() {
        m_acceptor.async_accept([this](beast::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::make_shared<Session>(std::move(socket), m_bind_apis)->start();
                // std::make_shared<WebSocketSession>(std::move(socket),
                // m_ioc)
                //     ->start();
            }

            // keep listening
            do_accept();
        });
    }

    void HttpServer::start_threads() {
        for (std::size_t i = 0; i < m_num_threads - 1; ++i) {
            m_threads.emplace_back([this, i] { m_ioc.run(); });
        }
        m_ioc.run();
        for (auto &t : m_threads) {
            t.join();
        }
    }

    //------------------------------------------------------------------------------

    result::Result start_service(const std::string_view host_address, uint16_t port, int threads_count) {
        spdlog::info("Start http daemon: {0}:{1}, with {2} thread(s)", host_address, port, threads_count);
        try {
            HttpServer server(host_address, port, threads_count);

            server.register_api("/test", models::Api{http::verb::get, [](const http::request<http::string_body> &, http::response<http::dynamic_body> &res,
                                                                         std::unordered_map<std::string_view, std::string_view>) {
                                                         write_json_result(http::status::ok, boost::json::value{{"result", "success"}}, res);
                                                     }});
            components::register_component_apis(server);
            server.run();
        } catch (const std::exception &e) {
            spdlog::error("Error: {0}", e.what());
            return result::Failed;
        }

        return result::Success;
    }
} // namespace http_service
