#pragma once
#include "Token.h"
#include "Types.h"
#include <unordered_map>

template <typename R>
class Environment {
public:
    void define(std::string& name, R& value) {
        _values[name] = value;
    }
    R get(Token& name) {
        if(_values.find(name._lexeme) != _values.end()) {
            return _values.at(name._lexeme);
        }
        throw RuntimeError(name, "Undefined variable '" + name._lexeme +"'.");
    }
    void assign(Token& name, R& value) {
        if(_values.find(name._lexeme) != _values.end()) {
            _values[name._lexeme] = value;
            return;
        }
        throw RuntimeError(name, "Undefined variable '" + name._lexeme +"'.");
    }

private:
    std::unordered_map<std::string, R> _values;
};