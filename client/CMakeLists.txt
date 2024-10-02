include_directories(${PROJECT_ROOT}/client/include)

aux_source_directory(${PROJECT_ROOT}/src BASE_SRC_FILES)
aux_source_directory(${PROJECT_ROOT}/client/src SRC_FILES)

add_executable(client ${BASE_SRC_FILES} ${SRC_FILES})


target_link_libraries(client PRIVATE Boost::thread Boost::chrono Boost::json Boost::system Boost::url)
target_link_libraries(client PRIVATE Boost::filesystem)
target_link_libraries(client PRIVATE spdlog::spdlog)
target_link_libraries(client PRIVATE yaml-cpp::yaml-cpp)
target_link_libraries(client PRIVATE OpenSSL::SSL OpenSSL::Crypto)