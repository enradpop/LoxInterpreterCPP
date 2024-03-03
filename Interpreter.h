#pragma once
#include "Expr.h"
#include "Stmt.h"
#include "Environment.h"
#include "Types.h"
#include <memory>
#include <vector>

class Interpreter : public Visitor<ReturnType>, StmtVisitor<ReturnType> {
public:
    ReturnType visitBinaryExpr(Binary<ReturnType>& expr) override;
    ReturnType visitGroupingExpr(Grouping<ReturnType>& expr) override;
    ReturnType visitLiteralExpr(Literal<ReturnType>& expr) override;
    ReturnType visitUnaryExpr(Unary<ReturnType>& expr) override;
    ReturnType visitVariableExpr(Variable<ReturnType>& expr) override;
    void checkNumberOperand(Token oprtr, ReturnType& operand);
    void checkNumberOperands(Token oprtr, ReturnType& left, ReturnType& right);
    void interpret(std::vector<Stmt<ReturnType>*> const& statements);
    void visitExpressionStmt(ExpressionStmt<ReturnType>& expression) override;
    void visitPrintStmt(Print<ReturnType>& print) override;
    void visitVarStmt(Var<ReturnType>& var) override;
private:
    ReturnType evaluate(Expr<ReturnType>& expr);
    void execute(Stmt<ReturnType>& stmt);
    bool isTruthy(ReturnType const& object);
    bool isEqual(ReturnType const& left, ReturnType const& right);
    std::string stringify(ReturnType const& object);
    Environment<ReturnType> _environment;
};