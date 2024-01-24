#include <iostream>
#include "Lox.h"
#include "Logger.h"

int main(int argc, char** argv) {
    std::string errStr = "Usage : clox ?debug [script] or simply clox for interactive mode\n";
    if (argc > 3) {
        std::cout << errStr;
    }
    else if (argc == 2) {
        if(std::string(argv[1]) == "--debug") {
            g_enableLogging = true;
            Lox::runPrompt();
        }
        else
            Lox::runFile(argv[1]);
    }
    else if (argc == 3) {
        if(std::string(argv[1]) == "--debug") {
            g_enableLogging = true;
            Lox::runFile(argv[2]);
        }
        else 
            std::cout << errStr;
    }
    else
        Lox::runPrompt();
}