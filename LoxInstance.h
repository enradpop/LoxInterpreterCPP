#pragma once
#include "LoxClass.h"
#include "Types.h"
#include "Logger.h"
#include <unordered_map>

class LoxInstance {
public:
    LoxInstance(LoxClass& klass): klass(klass) {LOG("new LoxInstance")}
    ~LoxInstance() {LOG("delete LoxInstance");}
    std::string toString() { return klass._name + " instance";}
    ExpressionValue get(Token const& name, ExpressionValue &thisPtr);
    void set(Token const& name, ExpressionValue const& value);
    LoxClass& klass;
private:
    std::unordered_map<std::string, ExpressionValue> _fields;
};