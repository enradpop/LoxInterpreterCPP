#pragma once
#include <string>
#include "LoxCallable.h"
class LoxClass : public LoxCallable {
public:
    LoxClass(std::string const& name) : _name(name) {}
    ExpressionValue call(Interpreter& interpreter, std::vector<ExpressionValue>& arguments) override;
    std::string toString() override { return _name;}
    int arity() override { return 0;}
    std::string _name;
    
};