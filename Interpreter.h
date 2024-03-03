#pragma once
#include "Expr.h"
#include "Stmt.h"
#include "Environment.h"
#include "Types.h"
#include <memory>
#include <vector>

class Interpreter : public Visitor<ReturnType>, StmtVisitor<ReturnType> {
public:
    Interpreter() : _environment(std::make_shared<Environment<ReturnType>>()) {}
    ReturnType visitBinaryExpr(Binary<ReturnType>& expr) override;
    ReturnType visitGroupingExpr(Grouping<ReturnType>& expr) override;
    ReturnType visitLiteralExpr(Literal<ReturnType>& expr) override;
    ReturnType visitUnaryExpr(Unary<ReturnType>& expr) override;
    ReturnType visitVariableExpr(Variable<ReturnType>& expr) override;
    ReturnType visitAssignExpr(Assign<ReturnType>& expr) override;
    void checkNumberOperand(Token oprtr, ReturnType& operand);
    void checkNumberOperands(Token oprtr, ReturnType& left, ReturnType& right);
    void interpret(std::vector<Stmt<ReturnType>*> const& statements);
    void visitExpressionStmt(ExpressionStmt<ReturnType>& expression) override;
    void visitPrintStmt(Print<ReturnType>& print) override;
    void visitVarStmt(Var<ReturnType>& var) override;
    void visitBlockStmt(Block<ReturnType>& block) override;
private:
    ReturnType evaluate(Expr<ReturnType>& expr);
    void execute(Stmt<ReturnType>& stmt);
    void executeBlock(std::vector<std::unique_ptr<Stmt<ReturnType>>>& statements, std::shared_ptr<Environment<ReturnType>>& environment);
    bool isTruthy(ReturnType const& object);
    bool isEqual(ReturnType const& left, ReturnType const& right);
    std::string stringify(ReturnType const& object);
    std::shared_ptr<Environment<ReturnType>> _environment;
};