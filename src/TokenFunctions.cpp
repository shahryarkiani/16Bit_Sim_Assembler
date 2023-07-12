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

std::vector<std::string> tokenizeFile(const std::string& filename) {

    std::vector<std::string> tokens;


    std::ifstream fileStream;
    fileStream.open(filename);

    if(!fileStream.is_open()) {
        std::cout << "Unable to open file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string curLine;

    //We will loop through the string, find beginnings and ends of tokens and put their indexes in here
    std::vector<std::pair<int, int>> breakpoints;

    while(std::getline(fileStream, curLine)) {

        auto newTokens = tokenizeString(curLine);
        tokens.insert(tokens.end(), newTokens.begin(), newTokens.end());

    }

    return tokens;

}

std::vector<std::string> tokenizeString(std::string& input) {

    std::vector<std::string> tokens;
    std::vector<std::pair<int, int>> breakpoints;

    input = trim(input);

    auto left = input.begin();

    while(left < input.end()) {

        //We move left until we find a character
        while(left < input.end() && isDelimiter(*left))
            left++;

        auto right = left;

        //We start at left and move right until we find a space
        while(right < input.end() && !isDelimiter(*right))
            right++;

        //Everything in between is a token
        breakpoints.emplace_back(left - input.begin(), right - input.begin());

        //We position left to the right of the token we just processed
        left = right;
    }

    for(auto tokenPair : breakpoints) {
        auto begin = tokenPair.first + input.begin();
        auto end = tokenPair.second + input.begin();

        tokens.emplace_back(begin, end);
    }

    return tokens;

}



bool isDelimiter(char ch) {
    return std::isspace(ch) || ch == ',';
}


