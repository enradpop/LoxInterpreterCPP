#include <iostream>
#include <fstream>
#include <sstream>
#include "Lox.h"
#include "Scanner.h"
#include "Token.h"
#include "Parser.h"
#include "AstPrinter.h"

bool Lox::_hadError = false;
bool Lox::_hadRuntimeError = false;

Interpreter Lox::_interpreter = Interpreter();
Resolver Lox::_resolver = Resolver(Lox::_interpreter);

bool Lox::runFile(std::string const & path) {
    std::ifstream file(path);
    if(file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        run(buffer.str());
        file.close();
        if(_hadError || _hadRuntimeError) {
            exit(1);
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
    Parser parser(tokens);
    //TODO: mem mgmt?
    // Expr<ExpressionValue>* expression = parser.parse<ExpressionValue>();
    std::vector<std::unique_ptr<Stmt<ExpressionValue>>> statements = parser.parse<ExpressionValue>();

    // Stop if there was a syntax error.
    if (_hadError) return;
    _resolver.resolve(statements);
    if (_hadError) return;
    _interpreter.interpret(statements);

    //delete statements[0];
    
    // AstPrinter printer;
    // std::cout << printer.print(expression) << "\n";
    // delete expression;
}

void Lox::error(int line, std::string message) {
    report(line, "", message);
}

void Lox::report(int line, std::string where, std::string message) {
    std::cout <<  "[line " << line << "] Error" << where << ": " << message << std::endl;
    _hadError = true;
}

void Lox::runtimeError(RuntimeError& error) {
    std::cout << error.what() << "\n[line " << error._token._line << "]\n";
    _hadRuntimeError == true;
}

void Lox::error(Token token, std::string const& message) {
    if (token._type == Token::END_OF_FILE) {
      report(token._line, " at end", message);
    } else {
      report(token._line, " at '" + token._lexeme + "'", message);
    }
}