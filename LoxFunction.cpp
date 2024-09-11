#include "LoxFunction.h"
#include "LoxInstance.h"

ExpressionValue LoxFunction::call(Interpreter& interpreter, std::vector<ExpressionValue>& arguments) {
    auto environment = std::make_shared<Environment<ExpressionValue>>(_environment);
    for(int i = 0; i < declaration.params.size(); i++) {
        environment->define(declaration.params[i]._lexeme, arguments[i]);
    }
    try {
        interpreter.executeBlock(declaration.body, environment);
    }
    catch (Return&  returnValue) {
        return returnValue.value;
    }
    std::string This = "this";
    if (_isInitializer) return _environment->getAt(0, This);
    return nullptr;
}

CallableObject LoxFunction::bind(ExpressionValue& instance) {
    auto environment = std::make_shared<Environment<ExpressionValue>>(_environment);
    environment->define("this", instance);
    std::shared_ptr<LoxFunction> func =  std::make_shared<LoxFunction>(declaration, environment, _isInitializer);
    return func;
}
