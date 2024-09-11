#pragma once
#include "Types.h"
#include "Logger.h"

class Interpreter;

class LoxCallable {
public:
    virtual int arity() = 0;
    virtual CallableObject bind(ExpressionValue& instance) { return nullptr; }
    virtual ExpressionValue call(Interpreter& interpreter, std::vector<ExpressionValue>& arguments) = 0;
    virtual std::string toString() = 0;
    LoxCallable() {LOG("new LoxCallable");}
    virtual ~LoxCallable() {LOG("delete LoxCallable");}
};