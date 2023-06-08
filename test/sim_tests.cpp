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

TEST_CASE("TEST LOAD WORD OPCODE") {
    //Store 2 in r2, then load M[r2 + imm] into r1, where imm = 3, and 0xABCD is stored at that location
    auto sim = Simulator({0x2902, 0xA503, 0, 0, 0, 0xABCD});
    sim.step();
    sim.step();
    REQUIRE(sim.getRegisters()[1] == 0xABCD);
}

TEST_CASE("TEST ZERO OUT REGISTER WITH LUI AFTER ADD-I") {
    //Stores 32 in r1, then loads upper immediate of 0, which should zero out r1
    auto sim = Simulator({0x24A0, 0x6400});
    sim.step();
    REQUIRE(sim.getRegisters()[1] == 32);//Check that it was loaded
    sim.step();
    REQUIRE(sim.getRegisters()[1] == 0);
}

TEST_CASE("TEST BRANCH WITH EQUAL REGISTERS") {
    //Loads 5 into r1 and r2, then compares and jumps to pc + 1 + 3 instructions ahead if r1 == r2
    //Once there, it adds 53 to r3
    auto sim = Simulator({0x2485, 0x2905, 0xC503, 0, 0, 0,  0x2DB5});
    sim.step();
    sim.step();
    sim.step();
    sim.step();
    REQUIRE(sim.getRegisters()[3] == 53);
}

TEST_CASE("TEST BRANCH WITH NOT EQUAL REGISTERS") {
    //Same as above, but doesn't branch and writes 35 to r3
    auto sim = Simulator({0x2485, 0x2906, 0xC503, 0x2DA3, 0, 0,  0x2DB5});
    sim.step();
    sim.step();
    sim.step();
    sim.step();
    REQUIRE(sim.getRegisters()[3] == 35);
}

TEST_CASE("TEST NAND OPCODE") {
    //Store 32 in r1, then 55 in r2, then r3 = r1 NAND r2
    auto sim = Simulator({0x24A0, 0x2937, 0x4C82});
    sim.step();
    sim.step();
    sim.step();
    REQUIRE(sim.getRegisters()[3] == 0x1F);
}