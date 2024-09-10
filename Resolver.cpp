#include "Resolver.h"
#include "Lox.h"

void Resolver::visitBlockStmt(Block<ExpressionValue>& block) {
    beginScope();
    resolve(block.statements);
    endScope();
}

void Resolver::visitClassStmt(Class<ExpressionValue>& stmt) {
    declare(stmt.name);
    define(stmt.name);
    beginScope();
    _scopes.back()->emplace("this", true);
    for(auto& method : stmt.methods) {//METHODS
        FunctionType declaration = FunctionType::METHOD;
        resolveFunction(*method, declaration);
    }
    endScope();
}

void Resolver::resolve(std::vector<std::unique_ptr<Stmt<ExpressionValue>>>& statements) {
    for(auto& s : statements) {
        resolve(*s);
    }
}

void Resolver::resolve(Stmt<ExpressionValue>& stmt) {
    stmt.accept(*this);
}

void Resolver::resolve(Expr<ExpressionValue>& expr) {
    expr.accept(*this);
}

void Resolver::beginScope() {
    _scopes.emplace_back(std::make_unique<std::unordered_map<std::string, bool>>());
}

void Resolver::endScope() {
    _scopes.pop_back();
}

void Resolver::visitVarStmt(Var<ExpressionValue>& var) {
    declare(var.name);
    if(var.initializer != nullptr) {
        resolve(*var.initializer);
    }
    define(var.name);
}

void Resolver::declare(Token& name) {
    if(_scopes.empty()) {
        return;
    }
    if(_scopes.back()->find(name._lexeme) != _scopes.back()->end()) {
        Lox::error(name, "Already a variable with this name in this scope.");
    }
    _scopes.back()->emplace(name._lexeme, false);
}

void Resolver::define(Token& name) {
    if(_scopes.empty()) {
        return;
    }
    (_scopes.back())->at(name._lexeme) = true;
}

ExpressionValue Resolver::visitVariableExpr(Variable<ExpressionValue>& expr) {
    if(!_scopes.empty()) {
        auto iter = _scopes.back()->find(expr.name._lexeme);   
        if (iter != _scopes.back()->end() &&iter->second == false) {
            Lox::error(expr.name, "Can't read local variable in its own initializer.");
        }
    }
    resolveLocal(expr, expr.name);
    return nullptr;
}

void Resolver::resolveLocal(Expr<ExpressionValue>& expr, Token name) {
    for (int i = _scopes.size() - 1; i >= 0; i--) {
        if (_scopes.at(i)->find(name._lexeme) != _scopes.at(i)->end()) {
            _interpreter.resolve(expr, _scopes.size() - 1 - i);
        return;
      }
    }
}

ExpressionValue Resolver::visitAssignExpr(Assign<ExpressionValue>& expr) {
    resolve(*expr.value);
    resolveLocal(expr, expr.name);
    return nullptr;
}

void Resolver::visitFunctionStmt(Function<ExpressionValue>& stmt) {
    declare(stmt.name);
    define(stmt.name);

    resolveFunction(stmt, FunctionType::FUNCTION);
}

void Resolver::resolveFunction(Function<ExpressionValue>& function, FunctionType type) {
    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;
    beginScope();
    for(Token& param : function.params) {
        declare(param);
        define(param);
    }
    resolve(function.body);
    endScope();
    currentFunction  = enclosingFunction;
}

void Resolver::visitExpressionStmt(ExpressionStmt<ExpressionValue>& expression) {
    resolve(*expression.expression);
}

void Resolver::visitIfStmt(If<ExpressionValue>& ifStmt) {
    resolve(*ifStmt.condition);
    resolve(*ifStmt.thenBranch);
    if (ifStmt.elseBranch != nullptr){
         resolve(*ifStmt.elseBranch);
    }
}

void Resolver::visitPrintStmt(Print<ExpressionValue>& print) {
    resolve(*print.expression);
}

void Resolver::visitReturnStmt(ReturnStmt<ExpressionValue>& returnStmt) {
    if (currentFunction == FunctionType::NONE) {
      Lox::error(returnStmt.keyword, "Can't return from top-level code.");
    }
    if (returnStmt.value != nullptr) {
      resolve(*returnStmt.value);
    }
}

void Resolver::visitWhileStmt(While<ExpressionValue>& stmt) {
    resolve(*stmt.condition);
    resolve(*stmt.body);
}

ExpressionValue Resolver::visitBinaryExpr(Binary<ExpressionValue>& expr) {
    resolve(*expr.left);
    resolve(*expr.right);
    return nullptr;
}

ExpressionValue Resolver::visitCallExpr(Call<ExpressionValue>& expr) {
    resolve(*expr.callee);
    for (auto& argument : expr.arguments) {
      resolve(*argument);
    }
    return nullptr;
}

ExpressionValue Resolver::visitGetExpr(Get<ExpressionValue>& expr) {
    resolve(*expr.object);
    return nullptr;
}

ExpressionValue Resolver::visitGroupingExpr(Grouping<ExpressionValue>& expr) {
    resolve(*expr.expression);
    return nullptr;
}

ExpressionValue Resolver::visitLiteralExpr(Literal<ExpressionValue>& expr) {
    return nullptr;
}

ExpressionValue Resolver::visitLogicalExpr(Logical<ExpressionValue>& expr) {
    resolve(*expr.left);
    resolve(*expr.right);
    return nullptr;
}

ExpressionValue Resolver::visitSetExpr(Set<ExpressionValue>& expr) {
    resolve(*expr.value);
    resolve(*expr.object);
    return nullptr;
}

ExpressionValue Resolver::visitThisExpr(This<ExpressionValue>& expr) {
    resolveLocal(expr, expr.keyword);
    return nullptr;
}

ExpressionValue Resolver::visitUnaryExpr(Unary<ExpressionValue>& expr) {
    resolve(*expr.right);
    return nullptr;
}