#pragma once
#include <string>
class LoxClass {
public:
    LoxClass(std::string const& name) : _name(name) {}
    std::string toString() { return _name;}
    std::string _name;
    
};