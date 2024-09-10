aux_source_directory(${PROJECT_ROOT}/server/src SRC_FILES)

add_executable(server ${SRC_FILES})

target_link_libraries(server PRIVATE ${PROJECT_EXTERNAL}/yaml-cpp/build/libyaml-cpp.a)