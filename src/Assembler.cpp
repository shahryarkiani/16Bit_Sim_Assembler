//
// Created by shahr on 5/19/2023.
//

#include "Assembler.h"

Assembler::Assembler() {}

std::vector<uint16_t> Assembler::assemble(const std::vector<std::string>& tokens) {

    //The first pass, this maps the symbols(like labels) to their actual position or value in the resulting machine code
    std::unordered_map<std::string, uint16_t> symbolsMap = mapSymbols(tokens);

    std::vector<uint16_t> machineCode;

    //The second pass then generates the machine code, while also replacing the symbols with their real values
    for (auto it = tokens.begin(); it != tokens.end(); it++) {

        std::string token = *it;

        //Nothing to generate for labels
        if (isLabel(token))
            continue;

        //We determine what type of operation it is
        auto operation = getOperation(token);

        int operandCount = getOperandCount(operation);

        if (it + operandCount >= tokens.end()) {
            throw std::invalid_argument("Not enough operands for operation " + token);
        }



        switch (operation) {
            case operation::ADD: {
                int regA = getRegisterNum(*(++it));
                int regB = getRegisterNum(*(++it));
                int regC = getRegisterNum(*(++it));

                uint16_t instruction = 0 | (regA << 10) | (regB << 7) | regC;

                machineCode.push_back(instruction);

                break;
            }
            case operation::ADDIMMD: {
                int regA = getRegisterNum(*(++it));
                int regB = getRegisterNum(*(++it));

                int immd;

                ++it;

                if(symbolsMap.count(*it) == 1) {
                    immd = symbolsMap[*it];
                    if(immd > 127) {
                        throw std::invalid_argument("Immediate value of " + *it + " is too large");
                    }
                }
                else {
                    try {
                        immd = stoi((*it));
                        if(immd > 63 || immd < -64) {
                            throw std::invalid_argument("Immediate value of " + *it + " is too large");
                        }
                    } catch (std::invalid_argument& err) {
                        throw std::invalid_argument("Unable to parse this immediate token: " + *it);
                    }
                }

                immd &= 0b1111111;

                uint16_t instruction = (1 << 13) | (regA << 10) | (regB << 7) | immd;

                machineCode.push_back(instruction);

                break;
            }
            case operation::NAND: {
                int regA = getRegisterNum(*(++it));
                int regB = getRegisterNum(*(++it));
                int regC = getRegisterNum(*(++it));

                uint16_t instruction = (0b010 <<13 ) | (regA << 10) | (regB << 7) | regC;

                machineCode.push_back(instruction);

                break;
            }
            case operation::LOADUPPERIMMD: {
                int regA = getRegisterNum(*(++it));

                int immd;

                try {
                    immd = stoi(*(++it));
                    if(immd > 1023) {
                        throw std::invalid_argument("Immediate value of " + *it + " is too large");
                    }
                } catch (std::invalid_argument& err) {
                    throw std::invalid_argument("Unable to parse this immediate token: " + *it);
                }

                break;
            }
        }

    }

    return machineCode;
}


std::unordered_map<std::string, uint16_t> Assembler::mapSymbols(const std::vector<std::string> &tokens) {

    std::unordered_map<std::string, uint16_t> symbolsMap;

    int position = 0;

    for (auto it = tokens.begin(); it != tokens.end();) {
        auto token = *it;
        if (isLabel(token)) {
            symbolsMap.insert({ getLabel(token), position });
            it++;
        } else {
            auto operation = getOperation(token);
            if (operation == operation::SPACE) {
                it++;

                //bounds check just in case
                if(it == tokens.end()) {
                    throw std::invalid_argument("[ERROR] EOF reached before finding argument for space statement");
                }

                int fillSpaces = std::stoi(*it);

                if (fillSpaces >= 65536)
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
    if (token.empty())
        throw std::invalid_argument("Cannot get operation of empty string");


    if (stringOpMap.count(token) != 0) {
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

    if (op == operation::SPACE)
        throw std::invalid_argument("Unable to determine size of space operation");

    if (op == operation::LOADIMMD)
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






