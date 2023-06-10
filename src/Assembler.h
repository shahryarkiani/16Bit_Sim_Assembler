//
// Created by shahr on 5/19/2023.
//

#ifndef TOY_ASSEMBLER_ASSEMBLER_H
#define TOY_ASSEMBLER_ASSEMBLER_H
#include <vector>
#include <cstdint>
#include <string>
#include <unordered_map>

enum class operation {
    ADD, ADDIMMD, NAND, LOADUPPERIMMD, STOREW, LOADW, BRANCHEQUAl, JUMPANDLINK,//
    NOOP, HALT, LOADLOWERIMMD, LOADIMMD, FILL, SPACE
};

class Assembler {
public:
    std::vector<uint16_t> assemble(const std::vector<std::string>& tokens);
private:
    std::unordered_map<std::string, uint16_t> mapSymbols(const std::vector<std::string>& tokens);
    operation getOperation(const std::string& token);
    bool isLabel(const std::string& token);
    std::string getLabel(const std::string& token);
    int getOperationLength(operation op);
};


#endif //TOY_ASSEMBLER_ASSEMBLER_H
