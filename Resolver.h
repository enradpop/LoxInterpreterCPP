#pragma once
#include "Expr.h"
#include "Stmt.h"
#include "Types.h"
#include "Interpreter.h"
#include <stack>

class Resolver : public Visitor<ExpressionValue>, StmtVisitor<ExpressionValue>{
public:
    enum class FunctionType : uint8_t {
        NONE,
        FUNCTION
    };
    Resolver(Interpreter& interpreter) : _interpreter(interpreter) {}

    void visitBlockStmt(Block<ExpressionValue>& block) override;
    void visitVarStmt(Var<ExpressionValue>& var) override;
    void visitFunctionStmt(Function<ExpressionValue>& stmt) override;
    void visitExpressionStmt(ExpressionStmt<ExpressionValue>& expression) override;
    void visitIfStmt(If<ExpressionValue>& ifStmt) override;
    void visitPrintStmt(Print<ExpressionValue>& print) override;
    void visitReturnStmt(ReturnStmt<ExpressionValue>& returnStmt) override;
    void visitWhileStmt(While<ExpressionValue>& stmt) override;
    ExpressionValue visitVariableExpr(Variable<ExpressionValue>& expr) override;
    ExpressionValue visitAssignExpr(Assign<ExpressionValue>& expr) override;
    ExpressionValue visitBinaryExpr(Binary<ExpressionValue>& expr) override;
    ExpressionValue visitCallExpr(Call<ExpressionValue>& expr) override;
    ExpressionValue visitGroupingExpr(Grouping<ExpressionValue>& expr) override;
    ExpressionValue visitLiteralExpr(Literal<ExpressionValue>& expr) override;
    ExpressionValue visitLogicalExpr(Logical<ExpressionValue>& expr) override;
    ExpressionValue visitUnaryExpr(Unary<ExpressionValue>& expr) override;
    void resolve(std::vector<std::unique_ptr<Stmt<ExpressionValue>>>& statements);
private:
    void resolve(Stmt<ExpressionValue>& stmt);
    void resolve(Expr<ExpressionValue>& expr);
    void beginScope();
    void endScope();
    void declare(Token& name);
    void define(Token& name);
    void resolveLocal(Expr<ExpressionValue>& expr, Token name);
    void resolveFunction(Function<ExpressionValue>& stmt, FunctionType type);

    Interpreter& _interpreter;
    std::vector<std::unique_ptr<std::unordered_map<std::string, bool>>> _scopes;
    FunctionType currentFunction = FunctionType::NONE;
};