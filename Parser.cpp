#include "Parser.h"

Token Parser::consume(Token::TokenType type, std::string message) {
    if(check(type)) return advance();
    throw error(peek(), message);
}

//TODO: exc type
ParseError* Parser::error(Token token, std::string message) {
    Lox::error(token, message);
    return new ParseError(message.c_str());
}

void Parser::synchronize() {
    advance();
    while (!isAtEnd()) {
        if (previous()._type == Token::SEMICOLON) return;
        switch (peek()._type) {
            case Token::CLASS:
            case Token::FUN:
            case Token::VAR:
            case Token::FOR:
            case Token::IF:
            case Token::WHILE:
            case Token::PRINT:
            case Token::RETURN:
                return;
            default:
                advance();
        }
    }
}

bool Parser::match(std::vector<Token::TokenType> const & types) {
    for(auto type : types) {
        if(check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(Token::TokenType type) {
    if(isAtEnd())
        return false;
    return (peek()._type == type);
}

Token  Parser::advance() {
    if(!isAtEnd())
        _current++;
    return previous();
}