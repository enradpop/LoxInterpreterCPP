#include <iostream>
#include "Lox.h"

int main(int argc, char** argv) {
    if (argc > 2) {
        std::cout << "Usage : clox [script] or simply clox for interactive mode\n";
    }
    else if (argc == 2) {
        Lox::runFile(argv[1]);
    }
    else {
        Lox::runPrompt(); 
    }
}