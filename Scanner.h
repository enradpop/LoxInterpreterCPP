#pragma once
#include <string>
#include <vector>
#include <map>
#include "Token.h"
class Scanner {
public:
    Scanner(std::string source) : _source(source) {}
    std::vector<Token> scanTokens();
    bool isAtEnd();
    void scanToken();
    char advance();
    void addToken(Token::TokenType type);
    void addToken(Token::TokenType type, void* literal);
    bool match(char expected);
    char peek();
    char peekNext();
    void stringLiteral();
    bool isDigit(char c);
    void numberLiteral();
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    void identifier();

private:

    static inline const std::map<std::string, Token::TokenType> _keywords = {
        {"and", Token::AND}, {"class", Token::CLASS}, {"else", Token::ELSE}, {"false", Token::FALSE},
        {"for", Token::FOR}, {"fun", Token::FUN}, {"if", Token::IF}, {"nil", Token::NIL}, {"or", Token::OR}, 
        {"print", Token::PRINT}, {"return", Token::RETURN}, {"super", Token::SUPER}, {"this", Token::THIS},
        {"true", Token::TRUE}, {"var", Token::VAR}, {"while", Token::WHILE} };
    std::string _source;
    std::vector<Token> _tokens;
    int _start = 0;
    int _current = 0;
    int _line = 1;

};