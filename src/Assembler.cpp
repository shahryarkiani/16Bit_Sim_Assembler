//
// Created by shahr on 5/19/2023.
//

#include "Assembler.h"

Assembler::Assembler() {}

std::vector<uint16_t> Assembler::assemble(const std::vector<std::string>& tokens) {

    //The first pass, this maps the symbols(like labels) to their actual position or value in the resulting machine code
    auto symbolsMap = mapSymbols(tokens);

    //The second pass then generates the machine code, while also replacing the symbols with their real values


    return std::vector<uint16_t>();
}


std::unordered_map<std::string, uint16_t> Assembler::mapSymbols(const std::vector<std::string> &tokens) {

    std::unordered_map<std::string, uint16_t> symbolsMap;

    int position = 0;

    for(auto it = tokens.begin(); it != tokens.end();) {
        auto token = *it;
        if(isLabel(token)) {
            symbolsMap.insert({ getLabel(token), position });
            it++;
        } else {
            auto operation = getOperation(token);
            if(operation == operation::SPACE) {
                it++;

                //bounds check just in case
                if(it == tokens.end()) {
                    throw std::invalid_argument("[ERROR] EOF reached before finding argument for fill statement");
                }

                int fillSpaces = std::stoi(*it);

                if(fillSpaces >= 65536)
                    throw std::invalid_argument("[ERROR] .space argument is larger than memory space");

                position += fillSpaces;
                it++;

            } else {
                int length = getOperationSize(operation);
                int opCount = getOperandCount(operation);
                position += length;
                it += opCount + 1;
            }
        }
    }

    return symbolsMap;
}

operation Assembler::getOperation(const std::string &token) {
    if(token.empty())
        throw std::invalid_argument("Cannot get operation of empty string");


    if(stringOpMap.count(token) != 0) {
        return stringOpMap.at(token);
    } else {
        throw std::invalid_argument("[" + token + "] is an invalid token");
    }

}
//A label looks this:
//  myLabel:
//          add  r2, r3, r4
bool Assembler::isLabel(const std::string &token) {
    return (token.find(':') != std::string::npos);
}



std::string Assembler::getLabel(const std::string &token) {
    return {token.begin(), token.begin() + token.find(':')};
}

int Assembler::getOperationSize(operation op) {

    if(op == operation::SPACE)
        throw std::invalid_argument("Unable to determine size of space operation");

    if(op == operation::LOADIMMD)
        return 2;
    else
        return 1;
}

int Assembler::getOperandCount(operation op) {
    switch (op) {
        case operation::ADD: case operation::ADDIMMD: case operation::NAND: case operation::STOREW:
        case operation::LOADW: case operation::BRANCHEQUAl: {}
            return 3;
        case operation::LOADUPPERIMMD: case operation::JUMPANDLINK:
        case operation::LOADLOWERIMMD: case operation::LOADIMMD:
            return 2;
        case operation::FILL: case operation::SPACE:
            return 1;
        default:
            return 0;
    }
}

std::vector<uint16_t> Assembler::assemble(const std::string &filename) {
    return {};
}






