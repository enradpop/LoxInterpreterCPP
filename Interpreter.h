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
    ExpressionValue call(Interpreter& interpreter, std::vector<ExpressionValue>& arguments) override;
    std::string toString() override;
};

class Interpreter : public Visitor<ExpressionValue>, StmtVisitor<ExpressionValue> {
public:
    Interpreter() : _environment(std::make_shared<Environment<ExpressionValue>>()), _globals(_environment) {
        ExpressionValue func(std::make_shared<Clock>());
        _globals->define("clock", func);
    }
    ExpressionValue visitBinaryExpr(Binary<ExpressionValue>& expr) override;
    ExpressionValue visitGroupingExpr(Grouping<ExpressionValue>& expr) override;
    ExpressionValue visitLiteralExpr(Literal<ExpressionValue>& expr) override;
    ExpressionValue visitLogicalExpr(Logical<ExpressionValue>& expr) override;
    ExpressionValue visitSetExpr(Set<ExpressionValue>& expr) override;
    ExpressionValue visitThisExpr(This<ExpressionValue>& expr) override;
    ExpressionValue visitUnaryExpr(Unary<ExpressionValue>& expr) override;
    ExpressionValue visitVariableExpr(Variable<ExpressionValue>& expr) override;
    ExpressionValue visitAssignExpr(Assign<ExpressionValue>& expr) override;
    ExpressionValue visitCallExpr(Call<ExpressionValue>& expr) override;
    ExpressionValue visitGetExpr(Get<ExpressionValue>& expr) override;
    void checkNumberOperand(Token oprtr, ExpressionValue& operand);
    void checkNumberOperands(Token oprtr, ExpressionValue& left, ExpressionValue& right);
    void interpret(std::vector<std::unique_ptr<Stmt<ExpressionValue>>> const& statements);
    void visitExpressionStmt(ExpressionStmt<ExpressionValue>& expression) override;
    void visitFunctionStmt(Function<ExpressionValue>& stmt) override;
    void visitPrintStmt(Print<ExpressionValue>& print) override;
    void visitReturnStmt(ReturnStmt<ExpressionValue>& returnStmt) override;
    void visitVarStmt(Var<ExpressionValue>& var) override;
    void visitWhileStmt(While<ExpressionValue>& stmt) override;
    void visitBlockStmt(Block<ExpressionValue>& block) override;
    void visitClassStmt(Class<ExpressionValue>& stmt) override;
    void visitIfStmt(If<ExpressionValue>& ifStmt) override;
    std::shared_ptr<Environment<ExpressionValue>> _environment;
    std::shared_ptr<Environment<ExpressionValue>> _globals;
    void executeBlock(std::vector<std::unique_ptr<Stmt<ExpressionValue>>>& statements, std::shared_ptr<Environment<ExpressionValue>>& environment);
    void resolve(Expr<ExpressionValue>& expr, int depth);
private:
    ExpressionValue evaluate(Expr<ExpressionValue>& expr);
    void execute(Stmt<ExpressionValue>& stmt);
    bool isTruthy(ExpressionValue const& object);
    bool isEqual(ExpressionValue const& left, ExpressionValue const& right);
    std::string stringify(ExpressionValue const& object);
    ExpressionValue lookUpVariable(Token& name, Expr<ExpressionValue>& expr);
    
    std::shared_ptr<Clock> _clock;
    std::unordered_map<Expr<ExpressionValue>*, int> _locals;
};