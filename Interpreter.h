#pragma once
#include "Expr.h"
#include <variant>
#include <memory>

class RuntimeError : public std::runtime_error {
public:
    RuntimeError(Token token, std::string const& message) : std::runtime_error(message.c_str()), _token(token)
    {}
    Token _token;
};

using ReturnType = std::variant<double, std::string, bool, std::nullptr_t>;

class Interpreter : public Visitor<ReturnType> {
public:
    ReturnType visitBinaryExpr(Binary<ReturnType>& expr) override;
    ReturnType visitGroupingExpr(Grouping<ReturnType>& expr) override;
    ReturnType visitLiteralExpr(Literal<ReturnType>& expr) override;
    ReturnType visitUnaryExpr(Unary<ReturnType>& expr) override;
    void checkNumberOperand(Token oprtr, ReturnType& operand);
    void checkNumberOperands(Token oprtr, ReturnType& left, ReturnType& right);
    void interpret(Expr<ReturnType>& expr);
private:
    ReturnType evaluate(Expr<ReturnType>& expr);
    bool isTruthy(ReturnType const& object);
    bool isEqual(ReturnType const& left, ReturnType const& right);
    std::string stringify(ReturnType const& object);
};