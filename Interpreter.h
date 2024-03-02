#pragma once
#include "Expr.h"
#include "Stmt.h"
#include <variant>
#include <memory>
#include <vector>

class RuntimeError : public std::runtime_error {
public:
    RuntimeError(Token token, std::string const& message) : std::runtime_error(message.c_str()), _token(token)
    {}
    Token _token;
};

using ReturnType = std::variant<double, std::string, bool, std::nullptr_t>;

class Interpreter : public Visitor<ReturnType>, StmtVisitor<ReturnType> {
public:
    ReturnType visitBinaryExpr(Binary<ReturnType>& expr) override;
    ReturnType visitGroupingExpr(Grouping<ReturnType>& expr) override;
    ReturnType visitLiteralExpr(Literal<ReturnType>& expr) override;
    ReturnType visitUnaryExpr(Unary<ReturnType>& expr) override;
    void checkNumberOperand(Token oprtr, ReturnType& operand);
    void checkNumberOperands(Token oprtr, ReturnType& left, ReturnType& right);
    void interpret(std::vector<Stmt<ReturnType>*> const& statements);
    void visitExpressionStmt(ExpressionStmt<ReturnType>& expression) override;
    void visitPrintStmt(Print<ReturnType>& print) override;
private:
    ReturnType evaluate(Expr<ReturnType>& expr);
    void execute(Stmt<ReturnType>& stmt);
    bool isTruthy(ReturnType const& object);
    bool isEqual(ReturnType const& left, ReturnType const& right);
    std::string stringify(ReturnType const& object);
};