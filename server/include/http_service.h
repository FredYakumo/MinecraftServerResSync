#ifndef HTTP_SERVICE_H
#define HTTP_SERVICE_H


#include "result.h"
#include <cstdint>
#include <string>
#include <string_view>


namespace http_service {
    result::Result start_service(const std::string_view host_address, uint16_t port, int threads_count);
}

#endif