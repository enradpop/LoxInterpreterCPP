#pragma once
#include <vector>
#include "Token.h"
#include "Expr.h"
#include "Lox.h"

class ParseError : public std::exception {
public:
    ParseError(const char* message) : errorMessage(message) {}
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }

private:
    std::string errorMessage;
};

class Parser {
public:
    Parser(std::vector<Token> const& tokens):_tokens(tokens)
    {}

    template<typename R>
    Expr<R>* parse() {
        try {
            return expression<R>();
        } catch (ParseError& error) {
            return nullptr;
        }
    }
private:
    //expression     → equality ;
    template<typename R>
    Expr<R>* expression() {
        return equality<R>();
    }
    //equality       → comparison ( ( "!=" | "==" ) comparison )* ;
    template<typename R>
    Expr<R>* equality() {
        Expr<R>* expr = comparison<R>();
        while(match({Token::BANG_EQUAL, Token::EQUAL_EQUAL})) {
            Token oprtr = previous();
            Expr<R>* right = comparison<R>();
            expr = new Binary<R>(expr, oprtr, right);//TODO mem mgmt
        }
        return expr;
    }
    //comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
    template<typename R>
    Expr<R>* comparison() {
        Expr<R>* expr = term<R>();
        while(match({Token::GREATER, Token::GREATER_EQUAL, Token::LESS, Token::LESS_EQUAL})) {
            Token oprtr = previous();
            Expr<R>* right = term<R>();
            expr = new Binary<R>(expr, oprtr, right);
        }
        return expr;
    }
    // term           → factor ( ( "-" | "+" ) factor )* ;
    template<typename R>
    Expr<R>* term() {
        Expr<R>* expr = factor<R>();
        while(match({Token::MINUS, Token::PLUS})) {
            Token oprtr = previous();
            Expr<R>* right = factor<R>();
            expr = new Binary<R>(expr, oprtr, right);
        }
        return expr;
    }
    // factor         → unary ( ( "/" | "*" ) unary )* ;
    template<typename R>
    Expr<R>* factor() {
        Expr<R>* expr = unary<R>();
        while(match({Token::SLASH, Token::STAR})) {
            Token oprtr = previous();
            Expr<R>* right = unary<R>();
            expr = new Binary<R>(expr, oprtr, right);
        }
        return expr;
    }
    // unary          → ( "!" | "-" ) unary
    //                | primary ;
    template<typename R>
    Expr<R>* unary() {
        if(match({Token::BANG, Token::MINUS})) {
            Token oprtr = previous();
            Expr<R>* right = unary<R>();
            return new Unary<R>(oprtr, right);
        }
        return primary<R>();
    }
    // primary        → NUMBER | STRING | "true" | "false" | "nil"
    //                | "(" expression ")" ;
    template<typename R>
    Expr<R>* primary() {
        if(match({Token::FALSE})) return new Literal<R>({Token::FALSE, "false", false, 1}); //TODO: line number?
        if(match({Token::TRUE})) return new Literal<R>({Token::TRUE, "true", true, 1});
        if(match({Token::NIL})) return new Literal<R>({Token::NIL, "nil", std::nullopt, 1});
        if(match({Token::NUMBER})) return new Literal<R>({Token::NUMBER, previous()._lexeme, previous()._literal, 1});
        if(match({Token::STRING})) return new Literal<R>({Token::STRING, previous()._lexeme, previous()._literal, 1});
        if(match({Token::LEFT_PAREN})) {
            Expr<R>* expr = expression<R>();
            consume(Token::RIGHT_PAREN, "Expect ')' after expression.");
            return new Grouping<R>(expr);
        }
        throw error(peek(), "Expect expression.");
    }
    Token consume(Token::TokenType type, std::string message);
    ParseError error(Token token, std::string message);
    void synchronize();
    bool match(std::vector<Token::TokenType> const & types);
    bool check(Token::TokenType type);
    Token advance();
    bool isAtEnd() {
        return peek()._type == Token::END_OF_FILE;
    }
    Token peek() {
        return _tokens[_current];
    }
    Token previous() {
        return _tokens[_current - 1];
    }


    std::vector<Token> _tokens;
    int _current = 0;
};