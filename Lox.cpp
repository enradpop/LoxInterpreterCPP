#include <iostream>
#include <fstream>
#include <sstream>
#include "Lox.h"
#include "Scanner.h"
#include "Token.h"

bool Lox::_hadError = false;

bool Lox::runFile(std::string const & path) {
    std::ifstream file(path);
    if(file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        run(buffer.str());
        file.close();
        if(_hadError) {
            return false;
        }
        return true;
    }
    else {
        std::cout << "could not open file\n";
        return false;
    }
}

void Lox::runPrompt() {
    while(1) {
        std::cout << "> ";
        std::string line;
        if (!std::getline(std::cin, line)) {
            break; // Exit the loop if no more input is available
        }
        run(line);
        _hadError = false;
    }
}

void Lox::run(std::string const & source) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();
    for(auto& token : tokens) {
        //print token
        std::cout << token.toString() << std::endl ;
    }
}

void Lox::error(int line, std::string message) {
    report(line, "", message);
}

void Lox::report(int line, std::string where, std::string message) {
    std::cout <<  "[line " << line << "] Error" << where << ": " << message << std::endl;
    _hadError = true;
}