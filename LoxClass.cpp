#include <string>
#include "LoxClass.h"
#include "LoxInstance.h"
#include "Types.h"
#include <vector>
#include <memory>

ExpressionValue LoxClass::call(Interpreter& interpreter, std::vector<ExpressionValue>& arguments) {
    auto instance = std::make_shared<LoxInstance>(*this);
    return instance;
}

CallableObject LoxClass::findMethod(std::string name) {
    auto value = methods.find(name);
    if(value != methods.end()) {
        return value->second;
    }
    return nullptr;
}