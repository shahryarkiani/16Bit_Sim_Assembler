#include <catch2/catch_test_macros.hpp>
#include <unordered_map>
#include "../src/Assembler.h"

TEST_CASE("SYMBOL MAP TEST") {

    auto assembler = Assembler();

    std::vector<std::string> tokens = {"start:", "add", "r1", "r2", "start", ".fill", "5", "end:", "halt"};

    std::unordered_map<std::string, uint16_t> expected = {{"start", 0}, {"end", 2}};

    auto actual = assembler.mapSymbols(tokens);

    REQUIRE(expected == actual);
}