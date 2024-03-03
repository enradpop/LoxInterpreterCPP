#pragma once
#include "Token.h"
#include "Types.h"
#include <unordered_map>
#include <memory>

template <typename R>
class Environment {
public:
    Environment() = default;
    Environment(std::shared_ptr<Environment>& enclosing): _enclosing(enclosing) {}
    void define(std::string& name, R& value) {
        _values[name] = value;
    }
    R get(Token& name) {
        if(_values.find(name._lexeme) != _values.end()) {
            return _values.at(name._lexeme);
        }
        if(_enclosing)
        {
            return _enclosing->get(name);
        }
        throw RuntimeError(name, "Undefined variable '" + name._lexeme +"'.");
    }
    void assign(Token& name, R& value) {
        if(_values.find(name._lexeme) != _values.end()) {
            _values[name._lexeme] = value;
            return;
        }
        if(_enclosing)
        {
            return _enclosing->assign(name, value);
        }
        throw RuntimeError(name, "Undefined variable '" + name._lexeme +"'.");
    }

private:
    std::unordered_map<std::string, R> _values;
    std::shared_ptr<Environment> _enclosing;
};