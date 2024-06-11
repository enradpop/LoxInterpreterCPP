#pragma once
#include "Token.h"
#include "Types.h"
#include <unordered_map>
#include <memory>
#include <iostream>

template <typename R>
class Environment {
public:
    Environment() = default;
    Environment(std::shared_ptr<Environment>& enclosing): _enclosing(enclosing) {}
    void define(std::string const& name, R& value) {
        _values[name] = value;
    }
    Environment<R>& ancestor(int distance) {
        Environment<R>* environment = this;
        for (int i = 0; i < distance; i++) {
            environment = environment->_enclosing.get(); 
        }
        return *environment;
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
    R getAt(int distance, std::string& name) {
        R value = ancestor(distance)._values[name];
        if(std::holds_alternative<double>(value)) {
            //std::cout << "getAt Name " << name << " value " << std::get<double>(value) << " distance " << distance << std::endl;
        }
        return value;
    }
    void assignAt(int distance, Token& name, R& value) {
        ancestor(distance)._values[name._lexeme] = value;
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