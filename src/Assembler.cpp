//
// Created by shahr on 5/19/2023.
//

#include "Assembler.h"

std::vector<uint16_t> Assembler::assemble(const std::vector<std::string>& tokens) {

    //The first pass, this maps the symbols(like labels) to their actual position or value in the resulting machine code
    auto symbolsMap = mapSymbols(tokens);

    //The second pass then generates the machine code, while also replacing the symbols with their real values


    return std::vector<uint16_t>();
}

std::unordered_map<std::string, uint16_t> Assembler::mapSymbols(const std::vector<std::string> &tokens) {

    std::unordered_map<std::string, uint16_t> symbolsMap;

    int position = 0;

    for(std::string token : tokens) {
        if(isLabel(token)) {
            symbolsMap.insert({ getLabel(token), position });
        } else {
            position += getOperationLength(getOperation(token));
        }
    }

    return symbolsMap;
}

operation Assembler::getOperation(const std::string &token) {
    //TODO
    return operation::ADD;
}

bool Assembler::isLabel(const std::string &token) {
    //TODO
    return token == "";
}

std::string Assembler::getLabel(const std::string &token) {
    //TODO
    return std::string();
}

int Assembler::getOperationLength(operation op) {
    return 0;
}




