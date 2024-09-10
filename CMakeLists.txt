cmake_minimum_required(VERSION 3.17)
project(ServerResourceSync)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_EXTERNAL ${PROJECT_ROOT}/external)

find_package(Boost 1.85.0 REQUIRED COMPONENTS json thread chrono)

include_directories(${PROJECT_ROOT}/include)
include_directories(${PROJECT_EXTERNAL}/spdlog/include)
include_directories(${PROJECT_EXTERNAL}/yaml-cpp/include)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(client)
add_subdirectory(server)