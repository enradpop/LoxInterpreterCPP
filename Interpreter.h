#pragma once
#include "Expr.h"
#include "Stmt.h"
#include "Environment.h"
#include "Types.h"
#include "LoxCallable.h"
#include <memory>
#include <vector>

class Clock : public LoxCallable {
public:
    int arity() override;
    ReturnType call(Interpreter& interpreter, std::vector<ReturnType>& arguments) override;
    std::string toString() override;
};

class Interpreter : public Visitor<ReturnType>, StmtVisitor<ReturnType> {
public:
    Interpreter() : _environment(std::make_shared<Environment<ReturnType>>()), _globals(_environment) {
        ReturnType func(std::make_shared<Clock>());
        _globals->define("clock", func);
    }
    ReturnType visitBinaryExpr(Binary<ReturnType>& expr) override;
    ReturnType visitGroupingExpr(Grouping<ReturnType>& expr) override;
    ReturnType visitLiteralExpr(Literal<ReturnType>& expr) override;
    ReturnType visitLogicalExpr(Logical<ReturnType>& expr) override;
    ReturnType visitUnaryExpr(Unary<ReturnType>& expr) override;
    ReturnType visitVariableExpr(Variable<ReturnType>& expr) override;
    ReturnType visitAssignExpr(Assign<ReturnType>& expr) override;
    ReturnType visitCallExpr(Call<ReturnType>& expr) override;
    void checkNumberOperand(Token oprtr, ReturnType& operand);
    void checkNumberOperands(Token oprtr, ReturnType& left, ReturnType& right);
    void interpret(std::vector<std::unique_ptr<Stmt<ReturnType>>> const& statements);
    void visitExpressionStmt(ExpressionStmt<ReturnType>& expression) override;
    void visitFunctionStmt(Function<ReturnType>& stmt) override;
    void visitPrintStmt(Print<ReturnType>& print) override;
    void visitVarStmt(Var<ReturnType>& var) override;
    void visitWhileStmt(While<ReturnType>& stmt) override;
    void visitBlockStmt(Block<ReturnType>& block) override;
    void visitIfStmt(If<ReturnType>& ifStmt) override;
    std::shared_ptr<Environment<ReturnType>> _environment;
    std::shared_ptr<Environment<ReturnType>> _globals;
    void executeBlock(std::vector<std::unique_ptr<Stmt<ReturnType>>>& statements, std::shared_ptr<Environment<ReturnType>>& environment);
private:
    ReturnType evaluate(Expr<ReturnType>& expr);
    void execute(Stmt<ReturnType>& stmt);
    bool isTruthy(ReturnType const& object);
    bool isEqual(ReturnType const& left, ReturnType const& right);
    std::string stringify(ReturnType const& object);
    
    std::shared_ptr<Clock> _clock;
};