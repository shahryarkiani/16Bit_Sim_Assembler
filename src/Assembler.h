//
// Created by shahr on 5/19/2023.
//

#ifndef TOY_ASSEMBLER_ASSEMBLER_H
#define TOY_ASSEMBLER_ASSEMBLER_H
#include <vector>
#include <cstdint>
#include <string>

class Assembler {
public:
    std::vector<uint16_t> assemble(std::vector<std::string> tokens);
};


#endif //TOY_ASSEMBLER_ASSEMBLER_H
