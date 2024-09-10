#ifndef HTTP_SERVICE_H
#define HTTP_SERVICE_H


#include "result.h"
#include <cstdint>


namespace http_service {
    result::Result start_service(const char *host_address, uint16_t port);
}

#endif