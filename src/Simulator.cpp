
#include "Simulator.h"
#include <iostream>

Simulator::Simulator(const std::vector<uint16_t> initMemory) {
    pc = 0;
    halted = false;
    if(initMemory.size() > memory.size())
        throw std::invalid_argument("Input Memory is too long");

    for(int i = 0; i < initMemory.size(); i++) {
        memory[i] = initMemory[i];
    }
}

int Simulator::step() {

    if(!isHalted()) {
        ir = memory[pc];
        pc = executeInstruction();
        registers[0] = 0;//This clears any writes to r0 from executeInstruction()
    }

}

uint16_t Simulator::executeInstruction() {

    auto opcode = getOpCode();

    switch(opcode) {
        case 0: {
            auto& regA = getRegA();
            auto& regB = getRegB();
            auto& regC = getRegC();
            regC = regA + regB;
            break;
        }
        case 1: {
            //TODO
        }
        case 2: {
            //TODO
        }
        case 3: {//Load Upper Immediate
            auto& regA = getRegA();
            auto immd = getUnsignedImmediate();
            immd = (immd << 6) & (0xFFC0);
            regA = immd;
            break;
        }
        case 4: {
            //TODO
        }
        case 5: {
            //TODO
        }
        case 6: {
            //TODO
        }
        case 7: {
            //TODO
        }
    }

    return pc + 1;

}

// Instruction Format: 100    100  010  0000
//                     opCode regA regB Immediate or regC
inline uint16_t Simulator::getOpCode() {
    return (ir >> 13) & 0b111;//Right shift by 13 and only read lowest 3 bits
}

inline uint16_t& Simulator::getRegA() {
    return registers[(ir >> 10) & 0b111];//Right shift to read regA
}

inline uint16_t& Simulator::getRegB() {
    return registers[(ir >> 7) &0b111];//Same for regB
}

inline uint16_t& Simulator::getRegC() {
    return registers[ir & 0b111];
}

int16_t Simulator::getSignedImmediate() {
    //We have to sign extend our 7-bit signed immediate to a 16-bit number
    return (ir & 0x7F) | ((ir & 0x40) ? 0xFF80 : 0 );
    //OR the lower 7 bits with either all 1s in the upper 9 bits(if the 7th bit is 1) or just 0(it's a positive number)
}

inline uint16_t Simulator::getUnsignedImmediate() {
    return (ir & 0x3FF);
}


const std::array<uint16_t, 65536>& Simulator::getMemory() {
    return memory;
}

const std::array<uint16_t, 8> &Simulator::getRegisters() {
    return registers;
}

bool Simulator::isHalted() {
    return halted;
}
