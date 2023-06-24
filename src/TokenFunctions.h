//
// Created by shahr on 6/10/2023.
//

#ifndef TOY_ASSEMBLER_TOKENFUNCTIONS_H
#define TOY_ASSEMBLER_TOKENFUNCTIONS_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

    std::string trim(std::string& token);
    std::vector<std::string> tokenize(const std::string& filename);

#endif //TOY_ASSEMBLER_TOKENFUNCTIONS_H
