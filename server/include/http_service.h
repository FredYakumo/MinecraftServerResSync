#ifndef HTTP_SERVICE_H
#define HTTP_SERVICE_H

#include "result.h"
#include <algorithm>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/status.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/verb.hpp>
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
namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace net = boost::asio;      // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

class Session : public std::enable_shared_from_this<Session> {
public:
  explicit Session(
      tcp::socket socket,
      const std::unordered_map<std::string, models::Api> &bind_apis)
      : m_socket(std::move(socket)), m_bind_apis(bind_apis) {}

  void start() { do_read(); }

private:
  void do_read() {
    auto self = shared_from_this();
    async_read(m_socket, m_buffer, m_req,
               [self](beast::error_code ec, std::size_t) {
                 if (!ec) {
                   self->handle_request();
                 }
               });
  }

  void handle_request() {
    const auto path = std::string{m_req.target()};
    spdlog::info("Get request from: {0}, path: {1}",
                 m_socket.remote_endpoint().address().to_string(), path);

    const auto thread_id = boost::this_thread::get_id();
    spdlog::debug("Thread id: {0}", thread_id_to_string(thread_id));
    http::response<http::string_body> res;
    // route
    if (const auto &api = m_bind_apis.find(path); api != m_bind_apis.cend()) {
      if (m_req.method() != api->second.method) {
        res.result(http::status::bad_request);
        res.set(http::field::content_type, "text/plain");

        res.body() =
            (boost::format("Bad request method: {}") % m_req.method()).str();
        res.prepare_payload();
      } else {
        api->second.api_function(m_req, res);
      }
    } else {
      res.result(http::status::not_found);
      res.body() = "404: Not Found";
      res.prepare_payload();
    }
    do_write(std::move(res));
  }

  void do_write(http::response<http::string_body> &&res) {
    auto sp_res =
        std::make_shared<http::response<http::string_body>>(std::move(res));
    auto self = shared_from_this();
    http::async_write(
        m_socket, *sp_res, [self, sp_res](beast::error_code ec, std::size_t) {
          if (!ec) {
            self->m_socket.shutdown(tcp::socket::shutdown_send, ec);
          }
        });
  }

  tcp::socket m_socket;
  beast::flat_buffer m_buffer;
  http::request<http::string_body> m_req;
  const std::unordered_map<std::string, models::Api> &m_bind_apis;
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
  void do_accept() {
    m_acceptor.async_accept([this](beast::error_code ec, tcp::socket socket) {
      if (!ec) {
        std::make_shared<Session>(std::move(socket), m_bind_apis)->start();
      }
      // 继续等待新的连接
      do_accept();
    });
  }

  void start_threads() {
    for (std::size_t i = 0; i < m_num_threads - 1; ++i) {
      m_threads.emplace_back([this, i] { m_ioc.run(); });
    }
    m_ioc.run(); // 主线程也参与
    for (auto &t : m_threads) {
      t.join();
    }
  }

  net::io_context m_ioc;    // IO Context
  tcp::acceptor m_acceptor; // acceptor
  std::size_t m_num_threads;
  std::vector<boost::thread> m_threads;
  std::unordered_map<std::string, models::Api> m_bind_apis;
};

void write_json_result(boost::json::value json,
                       http::response<http::string_body> &out_res) {
  out_res.result(http::status::ok);
  out_res.set(http::field::content_type, "application/json");
  out_res.body() = boost::json::serialize(std::move(json));

  out_res.prepare_payload();
}

result::Result start_service(const std::string_view host_address, uint16_t port,
                             int threads_count);
} // namespace http_service

#endif