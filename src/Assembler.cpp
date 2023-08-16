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
            case operation::ADDIMMD: case operation::STOREW: case operation::LOADW: case operation::BRANCHEQUAl: {
                int regA = getRegisterNum(*(++it));
                int regB = getRegisterNum(*(++it));

                int immd;

                ++it;

                int opcode;

                switch(operation) {
                    case operation::ADDIMMD:
                        opcode = 0b001;
                        break;
                    case operation::STOREW:
                        opcode = 0b100;
                        break;
                    case operation::LOADW:
                        opcode = 0b101;
                        break;
                    case operation::BRANCHEQUAl:
                        opcode = 0b110;
                        break;
                }

                if(symbolsMap.count(*it) == 1) {
                    immd = symbolsMap[*it];
                    if(operation == operation::BRANCHEQUAl) {
                        int dest = immd;
                        immd = dest - machineCode.size() - 1;
                    }
                    if(immd > 63 || immd < -64) {
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

                uint16_t instruction = (opcode << 13) | (regA << 10) | (regB << 7) | immd;

                machineCode.push_back(instruction);

                break;
            }
            case operation::NAND: {
                int regA = getRegisterNum(*(++it));
                int regB = getRegisterNum(*(++it));
                int regC = getRegisterNum(*(++it));

                uint16_t instruction = (0b010 << 13 ) | (regA << 10) | (regB << 7) | regC;

                machineCode.push_back(instruction);

                break;
            }
            case operation::LOADUPPERIMMD: {
                int regA = getRegisterNum(*(++it));

                int immd;

                try {
                    ++it;

                    if(symbolsMap.count(*it)) {
                        immd = symbolsMap[*it];
                    } else {
                        immd = std::stoi(*it);
                    }

                } catch (std::invalid_argument& err) {
                    throw std::invalid_argument("Unable to parse this immediate token: " + *it);
                }

                if(immd > 1023) {
                    throw std::invalid_argument("Immediate value of " + *it + " is too large");
                }

                uint16_t instruction = (0b011 << 13) | (regA << 10) | (immd);

                machineCode.push_back(instruction);

                break;
            }
            case operation::JUMPANDLINK: {
                int regA = getRegisterNum(*(++it));
                int regB = getRegisterNum(*(++it));

                uint16_t instruction = (0b111 << 13) | (regA << 10) | (regB << 7);

                machineCode.push_back(instruction);
                break;
            }
            case operation::NOOP: {
                machineCode.push_back(0);
                break;
            }
            case operation::HALT: {
                //halt is simply a jalr with r0, r0 for both registers and a non-zero immd field
                uint16_t instruction = (0b111 << 13) | 1;
                machineCode.push_back(instruction);
                break;
            }
            case operation::LOADLOWERIMMD: {
                int regA = getRegisterNum(*(++it));

                int immd;

                try {
                    ++it;

                    if(symbolsMap.count(*it)) {
                        immd = symbolsMap[*it];
                    } else {
                        immd = std::stoi(*it);
                    }
                } catch (std::invalid_argument& err) {
                    throw std::invalid_argument("Unable to parse this immediate token: " + *it);
                }

                if(immd > 63) {
                    throw std::invalid_argument("Immediate value of " + *it + " is too large for lli");
                }

                uint16_t instruction = (0b001 << 13) | (regA << 10) | (regA << 7) | immd;

                machineCode.push_back(instruction);

                break;
            }
            case operation::LOADIMMD: {
                int regA = getRegisterNum(*(++it));

                int immd;

                try {
                    ++it;
                    if(symbolsMap.count(*it)) {
                        immd = symbolsMap[*it];
                    } else {
                        immd = std::stoi(*it);
                    }
                } catch (std::invalid_argument& _) {
                    throw std::invalid_argument("Unable to parse this immediate token: " + *it);
                }

                if ((immd & (0xFFFF)) > (65535)) {
                    throw std::invalid_argument("Immediate value of " + *it + " is too large for movi instruction");
                }

                uint16_t upperInstruction = (0b011 << 13) | (regA << 10) | ((immd >> 6) & 0x3FF);
                uint16_t lowerInstruction = (0b001 << 13) | (regA << 10) | (regA << 7) | (immd & 0x3F);

                machineCode.push_back(upperInstruction);
                machineCode.push_back(lowerInstruction);
                break;
            }
            case operation::FILL: {
                int immd;

                try {
                    ++it;
                    if(symbolsMap.count(*it)) {
                        immd = symbolsMap[*it];
                    } else {
                        immd = std::stoi(*it);
                    }
                } catch (std::invalid_argument& _) {
                    throw std::invalid_argument("Unable to parse this immediate token: " + *it);
                }

                if((immd & 0xFFFF) > 65535) {
                    throw std::invalid_argument("Immediate value of " + *it + " is too large for fill instruction");
                }

                machineCode.push_back(immd);

                break;
            }
            case operation::SPACE: {
                int immd;

                try {
                    ++it;
                    if(symbolsMap.count(*it)) {
                        immd = symbolsMap[*it];
                    } else {
                        immd = std::stoi(*it);
                    }
                } catch (std::invalid_argument& _) {
                    throw std::invalid_argument("Unable to parse this immediate token: " + *it);
                }

                int remainingSpace = 65535 - immd;


                if((immd & 0xFFFF) > 65535 || remainingSpace < 0) {
                    throw std::invalid_argument("Immediate value of " + *it + " is too large for fill instruction");
                }

                machineCode.insert(machineCode.end(), immd, 0);


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


int Assembler::getRegisterNum(const std::string &regToken) {
    if (regToken.length() != 2 || regToken.at(0) != 'r' || (regToken.at(1) < '0' || regToken.at(1) > '7')) {
        throw std::invalid_argument(regToken + " is not a valid register");
    }
    //Register tokens should be in the form of r0, r1 etc., so we just return the number character minus '0'
    return regToken.at(1) - '0';

}






