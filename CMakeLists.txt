cmake_minimum_required(VERSION 3.17)
project(ServerResourceSync)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(EXPORT_COMPILE_COMMANDS)

add_subdirectory(client)
add_subdirectory(server)
