#pragma once
#include <vector>
#include "Token.h"
#include "Expr.h"
#include "Stmt.h"
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
    std::vector<std::unique_ptr<Stmt<R>>> parse() {
        try {
            std::vector<std::unique_ptr<Stmt<R>>> statements;
            while(!isAtEnd()) {
                statements.emplace_back(declaration<R>());
            }

            return statements;
        } catch (ParseError& error) {
            return {};
        }
    }
private:
    // program        → declaration* EOF ;
    // block          → "{" declaration* "}" ;
    template<typename R> 
    std::vector<std::unique_ptr<Stmt<R>>> block() {
        std::vector<std::unique_ptr<Stmt<R>>> statements;
        while (!check(Token::RIGHT_BRACE) && !isAtEnd()) {
            statements.emplace_back(declaration<R>());
        }

        consume(Token::RIGHT_BRACE, "Expect '}' after block.");
        return statements;
    }
    // declaration    → classDecl | funDecl | varDecl | statement 
    template<typename R>
    Stmt<R>* declaration() {
        try {
            if(match({Token::CLASS})) return classDeclaration<R>();
            if(match({Token::FUN})) return function<R>("function");
            if(match({Token::VAR})) return varDeclaration<R>();
            return statement<R>();
        }
        catch(ParseError& err) {
            synchronize();
            return nullptr;
        }
    }
    // classDecl      → "class" IDENTIFIER "{" function* "}" ;
    template<typename R>
    Class<R>* classDeclaration() {
        Token name = consume(Token::IDENTIFIER, "Expect class name.");
        consume(Token::LEFT_BRACE, "Expect '{' before class body.");
        std::vector<std::unique_ptr<Function<R>>> methods;
        while (!check(Token::RIGHT_BRACE) && !isAtEnd()) {
            methods.emplace_back(function<R>("method"));
        }
        consume(Token::RIGHT_BRACE, "Expect '}' after class body.");
        return new Class<R>(name, methods);
    }
    // statement      → exprStmt | forStmt | ifStmt | printStmt | returnStmt | whileStmt | block;
    template<typename R>
    Stmt<R>* statement() {
        if(match({Token::FOR}))
            return forStatement<R>();
        if(match({Token::IF}))
            return ifStatement<R>();
        if(match({Token::PRINT}))
            return printStatement<R>();
        if(match({Token::RETURN}))
            return returnStatement<R>();
        if(match({Token::WHILE}))
            return whileStatement<R>();
        if(match({Token::LEFT_BRACE})) {
            auto blockStatements = block<R>();
            return new Block<R>(std::move(blockStatements));
        }
        return expressionStatement<R>();
    }
    // forStmt        → "for" "(" ( varDecl | exprStmt | ";" ) expression? ";"expression? ")" statement ;
    template<typename R>
    Stmt<R>* forStatement() {
        consume(Token::LEFT_PAREN, "Expect '(' after 'for'.");
        Stmt<R>* initializer = nullptr;
        if(match({Token::SEMICOLON})) {
            ;
        }
        if(match({Token::VAR})) {
            initializer = varDeclaration<R>();
        }
        else {
            initializer = expressionStatement<R>();
        }
        Expr<R>* condition = nullptr;
        if (!check(Token::SEMICOLON)) {
            condition = expression<R>();
        }
        consume(Token::SEMICOLON, "Expect ';' after loop condition.");
        Expr<R>* increment = nullptr;
        if (!check(Token::RIGHT_PAREN)) {
            increment = expression<R>();
        }
        consume(Token::RIGHT_PAREN, "Expect ')' after for clauses.");
        Stmt<R>* body = statement<R>();
         if (increment != nullptr) {
            //body = new Stmt.Block(Arrays.asList(body, new Stmt.Expression(increment)));
            std::vector<std::unique_ptr<Stmt<R>>> bodyAndIncrement;
            bodyAndIncrement.emplace_back(body);
            bodyAndIncrement.emplace_back(new ExpressionStmt<R>(increment));
            body = new Block<R>(std::move(bodyAndIncrement));
        }

        if (condition == nullptr) condition =  new Literal<R>({Token::TRUE, "true", true, 1});
        body = new While<R>(condition, body);

        if (initializer != nullptr) {
            //body = new Stmt.Block(Arrays.asList(initializer, body));
            std::vector<std::unique_ptr<Stmt<R>>> withInitializer;
            withInitializer.emplace_back(initializer);
            withInitializer.emplace_back(body);
            body = new Block<R>(std::move(withInitializer));
        }

        return body;
    }

    // ifStmt         → "if" "(" expression ")" statement
    //                        ( "else" statement )? ;
    template<typename R>
    Stmt<R>* ifStatement() {
        consume(Token::LEFT_PAREN, "Expect '(' after 'if'.");
        Expr<R>* condition = expression<R>();
        consume(Token::RIGHT_PAREN, "Expect ')' after if condition.");
        Stmt<R>* thenBranch = statement<R>();
        Stmt<R>* elseBranch = nullptr;
        if (match({Token::ELSE})) {
            elseBranch = statement<R>();
        }
        return new If<R>(condition, thenBranch, elseBranch);
    }

    //Print          → print expression;
    template<typename R>
    Stmt<R>* printStatement() {
        Expr<R>* value = expression<R>();
        consume(Token::SEMICOLON, "Expect ';' after value.");
        return new Print<R>(value);
    }
    //returnStmt     → "return" expression? ";"
    template<typename R>
    Stmt<R>* returnStatement() {
        Token keyword = previous();
        Expr<R>* value = nullptr;
        if(!check(Token::SEMICOLON)) {
            value = expression<R>();
        }
        consume(Token::SEMICOLON, "Expect ';' after return value.");
        return new ReturnStmt<R>(keyword, value);
    }
    //whileStmt      → "while" "(" expression ")" statement
    template<typename R>
    Stmt<R>* whileStatement() {
        consume(Token::LEFT_PAREN, "Expect '(' after 'while'.");
        Expr<R>* condition = expression<R>();
        consume(Token::RIGHT_PAREN, "Expect ')' after condition.");
        Stmt<R>* body = statement<R>();
        return new While<R>(condition, body);
    }
    //ExpressionStmt     → expression;
    template<typename R>
    Stmt<R>* expressionStatement() {
        Expr<R>* value = expression<R>();
        consume(Token::SEMICOLON, "Expect ';' after value.");
        return new ExpressionStmt<R>(value);
    }
    // funDecl        → "fun" function
    // function       → IDENTIFIER "(" parameters? ")" block
    // parameters     → IDENTIFIER ( "," IDENTIFIER )*
    template<typename R>
    Function<R>* function(std::string const& kind) {
        Token name = consume(Token::IDENTIFIER, "Expect " + kind + " name.");
        consume(Token::LEFT_PAREN, "Expect '(' after " + kind + " name.");
        std::vector<Token> parameters;
        if (!check(Token::RIGHT_PAREN)) {
            do {
                if (parameters.size() >= 255) {
                    error(peek(), "Can't have more than 255 parameters.");
                }
                parameters.emplace_back(consume(Token::IDENTIFIER, "Expect parameter name."));
            } while (match({Token::COMMA}));
        }
        consume(Token::RIGHT_PAREN, "Expect ')' after parameters.");
        consume(Token::LEFT_BRACE, "Expect '{' before " + kind + " body.");
        auto body = block<R>();
        return new Function<R>(name, parameters, body);
    }
    //varDecl     → "var" IDENTIFIER ( "=" expression )? ";" ;
    template<typename R>
    Stmt<R>* varDeclaration() {
        Token name = consume(Token::IDENTIFIER, "Expect variable name.");
        Expr<R>* initializer = nullptr;
        if(match({Token::EQUAL})) {
            initializer = expression<R>();
        }
        consume(Token::SEMICOLON, "Expect ';' after variable declaration.");
        return new Var<R>(name, initializer);
    }
    //expression     → assignment ;
    template<typename R>
    Expr<R>* expression() {
        return assignment<R>();
    }
    //assignment     → ( call "." )? IDENTIFIER "=" assignment | logic_or
    template<typename R>
    Expr<R>* assignment() {
        Expr<R>* expr = logic_or<R>();
        if(match({Token::EQUAL})) {
            Token equals = previous();
            Expr<R>* value  = assignment<R>();
            if(expr->isLValue()) {
                Token name = static_cast<Variable<R>*>(expr)->name;
                delete expr;
                return new Assign<R>(name, value);
            }
            else if(expr->isGetter()) {
                Token name = static_cast<Get<R>*>(expr)->name;
                Expr<R>* object = static_cast<Get<R>*>(expr)->object.release();
                delete expr;
                return new Set<R>(object, value, name);
            }
            error(equals, "Invalid assignment target.");
        }
        return expr;
    }
    //logic_or       → logic_and ( "or" logic_and )*
    template<typename R>
    Expr<R>* logic_or() {
        Expr<R>* expr = logic_and<R>();
        while(match({Token::OR})) {
            Token oprtr = previous();
            Expr<R>* right = logic_and<R>();
            expr = new Logical<R>(expr, right, oprtr);
        }
        return expr;
    }
    //logic_and      → equality ( "and" equality )*
    template<typename R>
    Expr<R>* logic_and() {
        Expr<R>* expr = equality<R>();
        while(match({Token::AND})) {
            Token oprtr = previous();
            Expr<R>* right = equality<R>();
            expr = new Logical<R>(expr, right, oprtr);
        }
        return expr;
    }
    //equality       → comparison ( ( "!=" | "==" ) comparison )* ;
    template<typename R>
    Expr<R>* equality() {
        Expr<R>* expr = comparison<R>();
        while(match({Token::BANG_EQUAL, Token::EQUAL_EQUAL})) {
            Token oprtr = previous();
            Expr<R>* right = comparison<R>();
            expr = new Binary<R>(expr, oprtr, right);
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
    // unary          → ( "!" | "-" ) unary | call
    template<typename R>
    Expr<R>* unary() {
        if(match({Token::BANG, Token::MINUS})) {
            Token oprtr = previous();
            Expr<R>* right = unary<R>();
            return new Unary<R>(oprtr, right);
        }
        return call<R>();
    }
    // helper function to build call
    // arguments      → expression ( "," expression )*
    template<typename R>
    Expr<R>* finishCall(Expr<R>* callee) {
        std::vector<std::unique_ptr<Expr<R>>> arguments;
        if (!check(Token::RIGHT_PAREN)) {
            do {
                if(arguments.size() >= 255) {
                    error(peek(), "Can't have more than 255 arguments.");
                }
                arguments.emplace_back(expression<R>());
            } while (match({Token::COMMA}));
        }

        Token paren = consume(Token::RIGHT_PAREN, "Expect ')' after arguments.");

        return new Call<R>(callee, paren, arguments);
    }
    // call           → primary ( "(" arguments? ")" | "." IDENTIFIER )*
    template<typename R>
    Expr<R>* call() {
        Expr<R>* expr = primary<R>();

        while (true) { 
            if (match({Token::LEFT_PAREN})) {
                expr = finishCall<R>(expr);
            }
            else if (match({Token::DOT})) {
                Token name =  consume(Token::IDENTIFIER, "Expect property name after '.'.");
                expr = new Get<R>(expr, name);
            }
            else {
                break;
            }
        }

        return expr;
    }
    // primary        → NUMBER | STRING | "true" | "false" | "nil"
    //                | "(" expression ")" | IDENTIFIER ;
    template<typename R>
    Expr<R>* primary() {
        if(match({Token::FALSE})) return new Literal<R>({Token::FALSE, "false", false, 1}); //TODO: line number?
        if(match({Token::TRUE})) return new Literal<R>({Token::TRUE, "true", true, 1});
        if(match({Token::NIL})) return new Literal<R>({Token::NIL, "nil", nullptr, 1});
        if(match({Token::NUMBER})) return new Literal<R>({Token::NUMBER, previous()._lexeme, previous()._literal, 1});
        if(match({Token::STRING})) return new Literal<R>({Token::STRING, previous()._lexeme, previous()._literal, 1});
        if(match({Token::IDENTIFIER})) return new Variable<R>(previous());
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