include_directories(${PROJECT_ROOT}/server/include)

aux_source_directory(${PROJECT_ROOT}/server/src SRC_FILES)

add_executable(server ${SRC_FILES})

target_link_libraries(server PRIVATE Boost::thread Boost::chrono Boost::json Boost::system Boost::url)
target_link_libraries(server PRIVATE Boost::filesystem)
target_link_libraries(server PRIVATE spdlog::spdlog)
target_link_libraries(server PRIVATE yaml-cpp::yaml-cpp)
target_link_libraries(server PRIVATE OpenSSL::SSL OpenSSL::Crypto)