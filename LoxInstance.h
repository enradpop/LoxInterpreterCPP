#pragma once
#include "LoxClass.h"
#include "Types.h"
#include <unordered_map>
class LoxInstance {
public:
    LoxInstance(LoxClass& klass): klass(klass) {}
    std::string toString() { return klass._name + " instance";}
    ExpressionValue get(Token const& name);
    void set(Token const& name, ExpressionValue const& value);
    LoxClass& klass;
private:
    std::unordered_map<std::string, ExpressionValue> _fields;
};