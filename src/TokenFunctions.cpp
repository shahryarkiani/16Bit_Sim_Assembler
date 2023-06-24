//
// Created by shahr on 6/10/2023.
//
#include "TokenFunctions.h"


std::string trim(std::string& token) {//Trims leading and trailing whitespace
    auto left = token.begin();
    for(left; std::isspace(*left); ++left);

    auto right = token.end() - 1;
    for(right; std::isspace(*right); --right);

    if(left > right)
        return "";
    return std::string(left, right + 1);
}


