//
// Created by shahr on 5/19/2023.
//

#ifndef TOY_ASSEMBLER_ASSEMBLER_H
#define TOY_ASSEMBLER_ASSEMBLER_H
#include <vector>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <regex>
#include "TokenFunctions.h"


enum class operation {
    ADD, ADDIMMD, NAND, LOADUPPERIMMD, STOREW, LOADW, BRANCHEQUAl, JUMPANDLINK,//
    NOOP, HALT, LOADLOWERIMMD, LOADIMMD, FILL, SPACE
};

class Assembler {
public:
    Assembler();
    std::vector<uint16_t> assemble(const std::vector<std::string>& tokens);
    std::vector<uint16_t> assemble(const std::string& filename);
    std::unordered_map<std::string, uint16_t> mapSymbols(const std::vector<std::string>& tokens);
private:
    operation getOperation(const std::string& token);
    bool isLabel(const std::string& token);
    std::string getLabel(const std::string& token);
    int getOperationSize(operation op);
    int getOperandCount(operation op);
    int getRegisterNum(const std::string& regToken);
};

static std::unordered_map<std::string, operation> const stringOpMap = {
        {"add", operation::ADD},
        {"addi", operation::ADDIMMD},
        {"nand", operation::NAND},
        {"lui", operation::LOADUPPERIMMD},
        {"sw", operation::STOREW},
        {"lw", operation::LOADW},
        {"beq", operation::BRANCHEQUAl},
        {"jalr", operation::JUMPANDLINK},
        {"halt", operation::HALT},
        {"lli", operation::LOADLOWERIMMD},
        {"movi", operation::LOADIMMD},
        {".fill", operation::FILL},
        {".space", operation::SPACE},
        {"nop", operation::NOOP}
};

#endif //TOY_ASSEMBLER_ASSEMBLER_H
