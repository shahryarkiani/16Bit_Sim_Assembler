#ifndef TOY_ASSEMBLER_SIMULATOR_H
#define TOY_ASSEMBLER_SIMULATOR_H


#include <cstdint>
#include <string>
#include <array>
#include <vector>

class Simulator {
public:
    explicit Simulator(std::vector<uint16_t> initMemory);
    int step();
    std::string printRegisters();
    std::string printMemoryValue(uint16_t index);
    std::string printCurrentInstruction();
    bool isHalted();
    const std::array<uint16_t, 65536>& getMemory();
    const std::array<uint16_t, 8>& getRegisters();
private:
    uint16_t pc;
    uint16_t ir;
    std::array<uint16_t, 65536> memory{};//Memory is accessed in groups of 2 bytes, and with 16 bits, there are 2^16 accessible locations
    std::array<uint16_t, 8> registers{};//8 General Purpose Registers
    bool halted;

    inline uint16_t getOpCode();//The opcode is always in the upper 3 bits
    inline uint16_t& getRegA();//Return reference to RegA, next 3 bits
    inline uint16_t& getRegB();//RegB is in the next 3 bits
    inline uint16_t& getRegC();//RegC is in the lower 3 bits
    inline int16_t getSignedImmediate();//Lower 7 bits
    inline uint16_t getUnsignedImmediate();//Lower 10 bits
    uint16_t executeInstruction();//Returns the new pc after executing the instruction
};


#endif //TOY_ASSEMBLER_SIMULATOR_H
