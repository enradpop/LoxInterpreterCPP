#include "LoxInstance.h"

ExpressionValue LoxInstance::get(Token const& name) {
    auto value = _fields.find(name._lexeme);
    if(value != _fields.end()) {
        return value->second;
    }
    throw RuntimeError(name, "Undefined property '" + name._lexeme + "'.");
}

void LoxInstance::set(Token const& name, ExpressionValue const& value) {
    _fields[name._lexeme] = value;
}