cmake_minimum_required(VERSION 3.17)
project(MinecraftServerResSync)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_EXTERNAL ${PROJECT_ROOT}/external)
find_package(Boost 1.85.0 REQUIRED COMPONENTS json thread chrono filesystem system url)
# set(YAML_CPP_LIBRARY_DIR ${PROJECT_EXTERNAL}/yaml-cpp/build)
# set(yaml-cpp_DIR ${PROJECT_EXTERNAL}/yaml-cpp/build)
find_package(spdlog REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(yaml-cpp REQUIRED)

# include(FetchContent)

# FetchContent_Declare(
#   yaml-cpp
#   GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
#   GIT_TAG master # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
# )
# FetchContent_MakeAvailable(yaml-cpp)


include_directories(${PROJECT_ROOT}/include)
# include_directories(${PROJECT_EXTERNAL}/spdlog/include)
# include_directories(${PROJECT_EXTERNAL}/yaml-cpp/include)
# include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(client)
add_subdirectory(server)

enable_testing()

find_package(Catch2 REQUIRED)

add_executable(utils_test ${PROJECT_ROOT}/test/utils_test.cpp)
target_link_libraries(utils_test Catch2::Catch2WithMain)
target_link_libraries(utils_test OpenSSL::SSL OpenSSL::Crypto)
include(CTest)
include(Catch)

add_test(NAME utils_test COMMAND utils_test)