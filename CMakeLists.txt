cmake_minimum_required(VERSION 3.17)
project(ServerResourceSync)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# find_package(Boost 1.85.0 REQUIRED COMPONENTS beast json thread chrono)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/spdlog/include)

add_subdirectory(client)
add_subdirectory(server)