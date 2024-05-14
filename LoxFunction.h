#pragma once
#include "Types.h"
#include "Stmt.h"
#include "Environment.h"
#include "LoxCallable.h"
#include "Interpreter.h"

class LoxFunction : public LoxCallable {
public:
    LoxFunction(Function<ExpressionValue>& declaration, std::shared_ptr<Environment<ExpressionValue>>& closure) : declaration(declaration), _environment(closure){}
    ExpressionValue call(Interpreter& interpreter, std::vector<ExpressionValue>& arguments) override {
        auto environment = std::make_shared<Environment<ExpressionValue>>(_environment);
        for(int i = 0; i < declaration.params.size(); i++) {
            environment->define(declaration.params[i]._lexeme, arguments[i]);
        }
        try {
            interpreter.executeBlock(declaration.body, environment);
        }
        catch (Return&  returnValue) {
            return returnValue.value;
        }
        return nullptr;
    }
    int arity() override {
        return declaration.params.size();
    }
    std::string toString() override {
        return "<fn " + declaration.name._lexeme + ">";
    }
private:
    Function<ExpressionValue>& declaration;
    std::shared_ptr<Environment<ExpressionValue>> _environment;
};