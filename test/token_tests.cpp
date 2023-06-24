#include <catch2/catch_test_macros.hpp>
#include "../src/TokenFunctions.h"


TEST_CASE("TRIM TEST") {

    std::string test = " the label:     ";
    REQUIRE(trim(test) == "the label:");


    test = "                 ";
    REQUIRE(trim(test) == "");

    test = "";
    REQUIRE(trim(test) == "");

    test = "the label:";
    REQUIRE(trim(test) == "the label:");
}