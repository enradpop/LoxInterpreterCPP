#pragma once
#include "Types.h"

class Interpreter;

class LoxCallable {
public:
    virtual int arity() = 0;
    virtual ReturnType call(Interpreter& interpreter, std::vector<ReturnType>& arguments) = 0;
    virtual std::string toString() = 0;
};