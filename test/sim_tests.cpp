#include <catch2/catch_test_macros.hpp>
#include "../src/Simulator.h"

TEST_CASE("TEST LOAD UPPER IMMEDIATE") {
    auto sim = Simulator({0x6555});//An instruction to load 0x155 into upper 10 bits r1
    sim.step();
    REQUIRE(sim.getRegisters()[1] == (0x155 << 6));
}

TEST_CASE("TEST WRITE TO R0") {
    auto sim = Simulator({0x63FF});//Loads a value into r0, which should always be 0
    sim.step();
    REQUIRE(sim.getRegisters()[0] == (0));
}

TEST_CASE("TEST ADD TWO REGISTERS INTO THIRD") {
    auto sim = Simulator({0x6401, 0x6801, 0x503});//Loads the same value into r1 and r2, then does r3 = r1+r2
    sim.step();
    sim.step();
    sim.step();
    REQUIRE(sim.getRegisters()[3] == (2 * (1<<6)));
}

TEST_CASE("TEST ADD-I OPCODE") {
    auto sim = Simulator({0x6801, 0x255B});//Loads 64 into r2, then adds -37 to it and stores result in r1
    sim.step();
    sim.step();
    REQUIRE(sim.getRegisters()[1] == (27));
}

TEST_CASE("TEST STORE WORD OPCODE") {
    //Store 32 in r1, then 55 in r2, then load r2 into M[32 + imm], where imm = 63
    auto sim = Simulator({0x24A0, 0x2937, 0x88BF});
    sim.step();
    sim.step();
    sim.step();
    REQUIRE(sim.getMemory()[32 + 63] == 55);
}