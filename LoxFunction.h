#pragma once
#include "Types.h"
#include "Stmt.h"
#include "Environment.h"
#include "LoxCallable.h"
#include "Interpreter.h"

//TODO have to clean up return types; right now we are breaking the contract by instantiating Function with static return type;
class LoxFunction : public LoxCallable {
public:
    LoxFunction(Function<ReturnType>& declaration) : declaration(declaration){}
    ReturnType call(Interpreter& interpreter, std::vector<ReturnType>& arguments) override {
        auto environment = std::make_shared<Environment<ReturnType>>(interpreter._globals);
        for(int i = 0; i < declaration.params.size(); i++) {
            environment->define(declaration.params[i]._lexeme, arguments[i]);
        }
        interpreter.executeBlock(declaration.body, environment);
        return nullptr;
    }
    int arity() override {
        return declaration.params.size();
    }
    std::string toString() override {
        return "<fn " + declaration.name._lexeme + ">";
    }
private:
    Function<ReturnType>& declaration;
};