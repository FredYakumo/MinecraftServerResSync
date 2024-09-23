#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "http_service.h"
#include "models.h"

namespace components {
    using request =
        boost::beast::http::request<boost::beast::http::string_body>;
    using response =
        boost::beast::http::response<boost::beast::http::string_body>;
    using verb = boost::beast::http::verb;

    void register_component_apis(http_service::HttpServer &server);
} // namespace components

#endif