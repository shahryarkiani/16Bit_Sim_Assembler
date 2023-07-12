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

TEST_CASE("TOKENIZE TEST") {

    std::string test = " add r1, r2,-7   ";
    std::vector<std::string> expectedTokens = {"add", "r1", "r2", "-7"};

    REQUIRE(tokenizeString(test) == expectedTokens);

    test = "main: \n  add r1,r2,start\n.fill 5  ";
    expectedTokens = {"main:", "add", "r1", "r2", "start", ".fill", "5"};

    REQUIRE(tokenizeString(test) == expectedTokens);
}