#pragma once
#include <string>

class Token {
public:
    enum TokenType {
        //single character tokens
        LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
        COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
        //one or two char tokens
        BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL,
        GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,
        //literals
        IDENTIFIER, STRING, NUMBER,
        //keywords
        AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
        PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

        END_OF_FILE
    };
    Token(TokenType type, std::string const & lexeme, void* literal, int line) :
    _type(type), _lexeme(lexeme), _literal(literal), _line(line)
    {}

    std::string toString() {
        std::string literal;
        if (_type == STRING) {
            literal = *((std::string*)_literal);
        }
        else if (_type == NUMBER) {
            literal = std::to_string(*((double*)_literal));
        }
        return std::to_string(_type) +" " + _lexeme + " " + literal;
    }
    TokenType _type;
    std::string _lexeme;
    void * _literal; //TODO: figure out what does this represent
    int _line;


};