#pragma once
#include <memory>
#include "Token.h"
#include "Logger.h"
// forward declarations
template <typename R> class Expr;
template <typename R> class Binary;
template <typename R> class Grouping;
template <typename R> class Literal;
template <typename R> class Unary;
template <typename R> class Visitor;

template <typename R> class Expr {
public:
    virtual R accept(Visitor<R>& visitor) = 0;
    virtual ~Expr(){};
};

template <typename R> class Binary: public Expr<R> {
public:
    Binary(Expr<R>* left, Token const& oprtr, Expr<R>* right): left(left), oprtr(oprtr), right(right)
    {LOG("new Binary");}

    ~Binary() override {LOG("delete Binary")}

    R accept(Visitor<R>& visitor) override {
        return visitor.visitBinaryExpr(*this);
    }

    std::unique_ptr<Expr<R>> left;
    Token oprtr;
    std::unique_ptr<Expr<R>> right;
};

template <typename R> class Grouping: public Expr<R> {
public:
    Grouping(Expr<R>* expression): expression(expression)
    {LOG("new Grouping");}

    ~Grouping() override {LOG("delete Grouping")}

    R accept(Visitor<R>& visitor) override {
        return visitor.visitGroupingExpr(*this);
    }

    std::unique_ptr<Expr<R>> expression;
};

template <typename R> class Literal: public Expr<R> {
public:
    Literal(Token const& value): value(value)
    {LOG("new Literal");}

    ~Literal() override {LOG("delete Literal")}

    R accept(Visitor<R>& visitor) override {
        return visitor.visitLiteralExpr(*this);
    }

    Token value;
};

template <typename R> class Unary: public Expr<R> {
public:
    Unary(Token const& oprtr, Expr<R>* right): oprtr(oprtr), right(right)
    {LOG("new Unary");}

    ~Unary() override {LOG("delete Unary")}

    R accept(Visitor<R>& visitor) override {
        return visitor.visitUnaryExpr(*this);
    }

    Token oprtr;
    std::unique_ptr<Expr<R>> right;
};

template <typename R> class Variable : public Expr<R> {
public:
    Variable(Token const& name) : name(name) {}
    
    R accept(Visitor<R>& visitor) override {
        return visitor.visitVariableExpr(*this);
    }

    Token name;
};

template <typename R> class Visitor {
public:
    virtual R visitBinaryExpr(Binary<R>& expr) = 0;
    virtual R visitGroupingExpr(Grouping<R>& expr) = 0;
    virtual R visitLiteralExpr(Literal<R>& expr) = 0;
    virtual R visitUnaryExpr(Unary<R>& expr) = 0;
    virtual R visitVariableExpr(Variable<R>& expr) = 0;
};