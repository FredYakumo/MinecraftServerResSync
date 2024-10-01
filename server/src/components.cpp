#include "components.h"
#include "http_service.h"
#include "main.h"
#include <boost/url.hpp>

namespace components {
    using request = boost::beast::http::request<boost::beast::http::string_body>;
    using response = boost::beast::http::response<boost::beast::http::string_body>;
    using verb = boost::beast::http::verb;

    void get_resource_list(const request &req, response &response, const std::unordered_map<std::string_view, std::string_view> &param_map) {
        boost::json::array ret{};
        for (auto &it : g_server_data.get_const()->class_file_resource()->class_file_hash_map()) {
            boost::json::array file_list{};

            for (auto &file_it : it.second.file_hash_map()) {
                file_list.push_back(boost::json::value{file_it.first, file_it.second});
            }

            ret.push_back(boost::json::value{{"class_name", it.first}, {"file_list", file_list}});
        }

        http_service::write_json_result(ret, response);
    }

    void fetch_resource(const request &req, response &response, const std::unordered_map<std::string_view, std::string_view>& param_map) {
        boost::json::array ret{};
        for (auto &p_it : param_map) {
            boost::json::object val {};
            val["key"] = p_it.first;
            val["value"] = p_it.second;
            ret.push_back(val);
        }
        http_service::write_json_result(ret, response);
    }

    void register_component_apis(http_service::HttpServer &server) {
        server.register_api("/getResourceList", models::Api{verb::get, get_resource_list});
        server.register_api("/fetchResource", models::Api{verb::get, fetch_resource});
    }
} // namespace components