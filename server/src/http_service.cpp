#include "http_service.h"

namespace http_service {
namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

std::string thread_id_to_string(const boost::thread::id &thread_id) {
    std::ostringstream oss;
    oss << thread_id;
    return oss.str();
}

//------------------------------------------------------------------------------

result::Result start_service(const std::string_view host_address, uint16_t port,
                             int threads_count) {
    spdlog::info("Start http daemon: {0}:{1}, with {2} thread(s)", host_address,
                 port, threads_count);
    try {
        HttpServer server(host_address, port, threads_count);

        server.register_api(
            "/test",
            models::Api{beast::http::verb::get,
                        [](http::request<http::string_body>,
                           http::response<http::string_body> &res) {
                            write_json_result(
                                boost::json::value{{"result", "success"}}, res);
                        }});

        server.run();
    } catch (const std::exception &e) {
        spdlog::error("Error: {0}", e.what());
        return result::Failed;
    }

    return result::Success;
}
} // namespace http_service
