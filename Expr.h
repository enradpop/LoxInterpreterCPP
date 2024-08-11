#pragma once
#include <memory>
#include <vector>
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
    virtual bool isLValue() {return false;}
    virtual bool isGetter() {return false;}
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

template <typename R> class Call: public Expr<R> {
public:
    Call(Expr<R>* callee, Token const& paren, std::vector<std::unique_ptr<Expr<R>>>& arguments): callee(callee), paren(paren), arguments(std::move(arguments)) {
        LOG("new Call");
    }

    ~Call() override {LOG("delete Call")}

    R accept(Visitor<R>& visitor) override {
        return visitor.visitCallExpr(*this);
    }

    std::unique_ptr<Expr<R>> callee;
    Token paren;
    std::vector<std::unique_ptr<Expr<R>>> arguments;
};

template <typename R> class Get: public Expr<R> {
public:
    Get(Expr<R>* expression, Token const& name) : object(expression), name(name)
    {LOG("new Get");}
    ~Get() override {LOG("delete Get");}
    R accept(Visitor<R>& visitor) override {
        return visitor.visitGetExpr(*this);
    }
    bool isGetter() override { return true;}

    std::unique_ptr<Expr<R>> object;
    Token name;
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

template <typename R> class Logical: public Expr<R> {
public:
    Logical(Expr<R>* left, Expr<R>* right, Token const& oprtr): left(left), right(right), oprtr(oprtr)
    {LOG("new Logical");}
    
    ~Logical() override {LOG("delete Logical");}

    R accept(Visitor<R>& visitor) override {
        return visitor.visitLogicalExpr(*this);
    }

    std::unique_ptr<Expr<R>> left, right;
    Token oprtr;
};

template <typename R> class Set: public Expr<R> {
public:
    Set(Expr<R>* object, Expr<R>* value,  Token const& name) : object(object), value(value), name(name)
    {LOG("new Set");}
    ~Set() override {LOG("delete Set");}
    R accept(Visitor<R>& visitor) override {
        return visitor.visitSetExpr(*this);
    }
    std::unique_ptr<Expr<R>> object, value;
    Token name;
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
    Variable(Token const& name) : name(name)
    {LOG("new Variable");}
    ~Variable() override {LOG("delete Variable")}
    
    R accept(Visitor<R>& visitor) override {
        return visitor.visitVariableExpr(*this);
    }

    bool isLValue() override {return true;}

    Token name;
};

template <typename R> class Assign : public Expr<R> {
public:
    Assign(Token const& name, Expr<R>* value) : name(name), value(value)
    {LOG("new Assign");}
    ~Assign() override {LOG("delete Assign")}
    R accept(Visitor<R>& visitor) override {
        return visitor.visitAssignExpr(*this);
    }
    Token name;
    std::unique_ptr<Expr<R>> value;
};

template <typename R> class Visitor {
public:
    virtual R visitBinaryExpr(Binary<R>& expr) = 0;
    virtual R visitCallExpr(Call<R>& expr) = 0;
    virtual R visitGetExpr(Get<R>& expr) = 0;
    virtual R visitGroupingExpr(Grouping<R>& expr) = 0;
    virtual R visitLogicalExpr(Logical<R>& expr) = 0;
    virtual R visitSetExpr(Set<R>& expr) = 0;
    virtual R visitLiteralExpr(Literal<R>& expr) = 0;
    virtual R visitUnaryExpr(Unary<R>& expr) = 0;
    virtual R visitVariableExpr(Variable<R>& expr) = 0;
    virtual R visitAssignExpr(Assign<R>& expr) = 0;
};