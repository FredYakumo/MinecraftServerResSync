#include "components.h"
#include "http_service.h"
#include "main.h"
#include <boost/url.hpp>
#include <fstream>

namespace components {
    using request = boost::beast::http::request<boost::beast::http::string_body>;
    using response = boost::beast::http::response<boost::beast::http::dynamic_body>;
    using verb = boost::beast::http::verb;

    void write_file_content_to_response(const std::string_view file_name, response &response) {
        spdlog::info("Reading file content: {}", file_name);
        const auto fs = std::ifstream {(file_name.data()), std::ios::binary};
        if (!fs.is_open()) {
            spdlog::error("File {} resources not found.", file_name);
            http_service::write_json_result(boost::beast::http::status::internal_server_error, boost::json::value{{"message", "Internal Service error"}}, response);
            return;
        }

        response.result(boost::beast::http::status::ok);
        response.set(boost::beast::http::field::content_disposition, fmt::format("attachment; filename=\"{}\"", file_name));
        ostream(response.body()) << fs.rdbuf();
        response.prepare_payload();
    }

    void get_resource_list(const request &req, response &response, const std::unordered_map<std::string_view, std::string_view> &param_map) {
        boost::json::array ret{};
        for (const auto &[fst, snd] : g_server_data.get_const()->class_file_resource()->class_file_hash_map()) {
            boost::json::array file_list{};

            for (const auto &[fst, snd] : snd.file_hash_map()) {
                file_list.push_back(boost::json::value{fst, snd});
            }

            ret.push_back(boost::json::value{{"class_name", fst}, {"file_list", file_list}});
        }

        http_service::write_json_result(boost::beast::http::status::ok, ret, response);
    }

    void fetch_resource(const request &req, response &response, const std::unordered_map<std::string_view, std::string_view> &param_map) {
        boost::json::array ret{};
        const auto file_key = param_map.find("file");
        if (file_key == std::cend(param_map)) {
            http_service::write_json_result(boost::beast::http::status::bad_request, boost::json::value{{"message", "file name param not provide"}}, response);
            return;
        }
        const auto class_key = param_map.find("class");
        if (class_key == std::cend(param_map)) {
            http_service::write_json_result(boost::beast::http::status::bad_request, boost::json::value{{"message", "class name param not provide"}}, response);
            return;
        }
        const auto class_name = std::string{class_key->second};
        const auto &class_file_hash_map = g_server_data.get_const()->class_file_resource()->class_file_hash_map();
        const auto &class_map = class_file_hash_map.find(class_name);
        if (class_map == std::cend(class_file_hash_map)) {
            http_service::write_json_result(boost::beast::http::status::not_found, boost::json::value{{"message", fmt::format("Class name '{}' not found.", class_name)}}, response);
            return;
        }
        const auto file_name = std::string{file_key->second};
        if (const auto &file_iter = class_map->second.file_hash_map().find(file_name); file_iter == std::cend(class_map->second.file_hash_map())) {
            http_service::write_json_result(boost::beast::http::status::not_found, boost::json::value{{"message", fmt::format("File {} is not in class {}", file_name, class_name)}}, response);
            return;
        }

        write_file_content_to_response(file_name, response);
    }


    void register_component_apis(http_service::HttpServer &server) {
        server.register_api("/getResourceList", models::Api{verb::get, get_resource_list});
        server.register_api("/fetchResource", models::Api{verb::get, fetch_resource});
    }
} // namespace components