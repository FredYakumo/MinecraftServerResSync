#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "console"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "utils.hpp"

TEST_CASE("Calculate sha1 str", "[utils::calculate_str_sha1]") {
    std::string source = "123456";
    auto sha1 = utils::calculate_str_sha1(source.c_str());
    INFO("source: " << source << ", sha1: " << sha1);
    REQUIRE(sha1 == "7c4a8d09ca3762af61e59520943dc26494f8941b");
    // CHECK(false);
}