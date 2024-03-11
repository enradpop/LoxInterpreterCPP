#pragma once
#include <memory>
#include <vector>
#include "Token.h"
#include "Logger.h"
// forward declarations
template <typename R> class Stmt;
template <typename R> class ExpressionStmt;
template <typename R> class Print;
template <typename R> class StmtVisitor;

template <typename R> class Stmt {
public:
    virtual void accept(StmtVisitor<R>& visitor) = 0;
    virtual ~Stmt(){};
};

template <typename R> class ExpressionStmt: public Stmt<R> {
public:
    ExpressionStmt(Expr<R>* expression): expression(expression)
    {LOG("new ExpressionStmt");}

    ~ExpressionStmt() override {LOG("delete ExpressionStmt")}

    void accept(StmtVisitor<R>& visitor) override {
        return visitor.visitExpressionStmt(*this);
    }

    std::unique_ptr<Expr<R>> expression;
};

template <typename R> class Print: public Stmt<R> {
public:
    Print(Expr<R>* expression): expression(expression)
    {LOG("new Print");}

    ~Print() override {LOG("delete Print")}

    void accept(StmtVisitor<R>& visitor) override {
        return visitor.visitPrintStmt(*this);
    }

    std::unique_ptr<Expr<R>> expression;
};

template<typename R> class Var : public Stmt<R> {
public:
    Var(Token name, Expr<R>* initializer) : name(name), initializer(initializer)
    {}
    void accept(StmtVisitor<R>& visitor) override {
        visitor.visitVarStmt(*this);
    }

    Token name;
    std::unique_ptr<Expr<R>> initializer;
};

template<typename R> class Block : public Stmt<R> {
public:
    Block(std::vector<std::unique_ptr<Stmt<R>>>& statements) : statements(statements) {}
    Block(std::vector<std::unique_ptr<Stmt<R>>>&& statements) : statements(std::move(statements)) {}
    void accept(StmtVisitor<R>& visitor) override {
        visitor.visitBlockStmt(*this);
    }
    std::vector<std::unique_ptr<Stmt<R>>> statements;
};

template<typename R> class If : public Stmt<R> {
public:
    If(Expr<R>* condition, Stmt<R>* thenBranch, Stmt<R>* elseBranch)
    : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch)
    {}

    void accept(StmtVisitor<R>& visitor) override {
        visitor.visitIfStmt(*this);
    }
    std::unique_ptr<Expr<R>> condition;
    std::unique_ptr<Stmt<R>> thenBranch, elseBranch;

};

template <typename R> class StmtVisitor {
public:
    virtual void visitExpressionStmt(ExpressionStmt<R>& stmt) = 0;
    virtual void visitPrintStmt(Print<R>& stmt) = 0;
    virtual void visitVarStmt(Var<R>& stmt) = 0;
    virtual void visitBlockStmt(Block<R>& stmt) = 0;
    virtual void visitIfStmt(If<R>& stmt) = 0;
};