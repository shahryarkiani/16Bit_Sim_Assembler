#include <catch2/catch_test_macros.hpp>
#include <unordered_map>
#include "../src/Assembler.h"
#include "../src/Simulator.h"

TEST_CASE("SYMBOL MAP TEST") {

    auto assembler = Assembler();

    std::vector<std::string> tokens = {"start:", "add", "r1", "r2", "start", ".fill", "5", "end:", "halt"};

    std::unordered_map<std::string, uint16_t> expected = {{"start", 0}, {"end", 2}};

    auto actual = assembler.mapSymbols(tokens);

    REQUIRE(expected == actual);
}

TEST_CASE("BASIC ASSEMBLY TEST") {


    auto assembler = Assembler();

    std::string asm_code = "movi r1, 50 add r1, r1, r1 lli r2, 25  sw r1, r2, 0 halt";

    auto tokens = tokenizeString(asm_code);

    auto machineCode = assembler.assemble(tokens);

    auto simulator = Simulator(machineCode);

    while(!simulator.isHalted()) {
        simulator.step();
    }

    REQUIRE(simulator.getMemory()[25] == 100);

    REQUIRE(simulator.getRegisters()[1] == 100);
    REQUIRE(simulator.getRegisters()[2] == 25);

}