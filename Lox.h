#pragma once
#include "Scanner.h"
#include "Token.h"

class Lox {
public:
    static bool runFile(std::string const & path);
    static void runPrompt();
    static void run(std::string const & source);
    static void error(int line, std::string message);
    static void report(int line, std::string where, std::string message);
private:
    static bool _hadError;
};