#pragma once
#include "Scanner.h"
#include "Token.h"
#include "Interpreter.h"
#include "Resolver.h"

class Lox {
public:
    static bool runFile(std::string const & path);
    static void runPrompt();
    static void run(std::string const & source);
    static void error(int line, std::string message);
    static void report(int line, std::string where, std::string message);
    static void error(Token token, std::string const& message);
    static void runtimeError(RuntimeError& error);
private:
    static bool _hadError;
    static bool _hadRuntimeError;
    static Interpreter _interpreter;
    static Resolver _resolver;
};