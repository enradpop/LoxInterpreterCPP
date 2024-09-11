#pragma once
#include <string>
#include "LoxFunction.h"
class LoxClass : public LoxCallable {
public:
    LoxClass(std::string const& name, std::unordered_map<std::string, std::shared_ptr<LoxFunction>>& methods) : _name(name), methods(std::move(methods)) {}
    ExpressionValue call(Interpreter& interpreter, std::vector<ExpressionValue>& arguments) override;
    std::string toString() override { return _name;}
    CallableObject findMethod(std::string name);
    int arity() override;
    std::string _name;
    std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods;
};