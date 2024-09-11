#include <string>
#include "LoxClass.h"
#include "LoxInstance.h"
#include "Types.h"
#include <vector>
#include <memory>

ExpressionValue LoxClass::call(Interpreter& interpreter, std::vector<ExpressionValue>& arguments) {
    ExpressionValue instance = std::make_shared<LoxInstance>(*this);
    CallableObject initializer = findMethod("init");
    if(initializer) {
        initializer->bind(instance)->call(interpreter, arguments);
    }
    return instance;
}

CallableObject LoxClass::findMethod(std::string name) {
    auto value = methods.find(name);
    if(value != methods.end()) {
        return value->second;
    }
    return nullptr;
}

int LoxClass::arity() {
    CallableObject initializer = findMethod("init");
    if (!initializer) return 0;
    return initializer->arity();
}