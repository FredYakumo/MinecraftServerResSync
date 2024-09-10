cmake_minimum_required(VERSION 3.17)
project(ServerResourceSync)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_EXTERNAL ${PROJECT_ROOT}/external)

cmake_policy(SET CMP0167 NEW)

find_package(Boost 1.85.0 REQUIRED COMPONENTS json thread chrono)
# set(YAML_CPP_LIBRARY_DIR ${PROJECT_EXTERNAL}/yaml-cpp/build)
# set(yaml-cpp_DIR ${PROJECT_EXTERNAL}/yaml-cpp/build)
find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)

# include(FetchContent)

# FetchContent_Declare(
#   yaml-cpp
#   GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
#   GIT_TAG master # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
# )
# FetchContent_MakeAvailable(yaml-cpp)


include_directories(${PROJECT_ROOT}/include)
include_directories(${PROJECT_EXTERNAL}/spdlog/include)
include_directories(${PROJECT_EXTERNAL}/yaml-cpp/include)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(client)
add_subdirectory(server)