#pragma once
#include "Types.h"
#include "Stmt.h"
#include "Environment.h"
#include "LoxCallable.h"
#include "Interpreter.h"
#include "Logger.h"

class LoxFunction : public LoxCallable {
public:
    LoxFunction(Function<ExpressionValue>& declaration, std::shared_ptr<Environment<ExpressionValue>>& closure, bool isInitializer) : 
    declaration(declaration), _environment(closure), _isInitializer(isInitializer) {
        LOG("new LoxFunction")
    }
    ~LoxFunction() override {LOG("delete LoxFunction")};
    ExpressionValue call(Interpreter& interpreter, std::vector<ExpressionValue>& arguments) override;
    CallableObject bind(ExpressionValue& instance) override;
    int arity() override {
        return declaration.params.size();
    }
    std::string toString() override {
        return "<fn " + declaration.name._lexeme + ">";
    }
private:
    Function<ExpressionValue>& declaration;
    std::shared_ptr<Environment<ExpressionValue>> _environment;
    bool _isInitializer;
};