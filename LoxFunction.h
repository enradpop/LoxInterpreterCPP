#pragma once
#include "Types.h"
#include "Stmt.h"
#include "Environment.h"
#include "LoxCallable.h"
#include "Interpreter.h"

//TODO have to clean up return types; right now we are breaking the contract by instantiating Function with static return type;
class LoxFunction : public LoxCallable {
public:
    LoxFunction(Function<ExpressionValue>& declaration) : declaration(declaration){}
    ExpressionValue call(Interpreter& interpreter, std::vector<ExpressionValue>& arguments) override {
        auto environment = std::make_shared<Environment<ExpressionValue>>(interpreter._globals);
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
};