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