#pragma once
#include "LoxClass.h"
class LoxInstance {
public:
    LoxInstance(LoxClass& klass): klass(klass) {}
    std::string toString() { return klass._name + " instance";}
    LoxClass& klass;
};