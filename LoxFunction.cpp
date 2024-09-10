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
    return nullptr;
}

ExpressionValue LoxFunction::bind(ExpressionValue& instance) {
    auto environment = std::make_shared<Environment<ExpressionValue>>(_environment);
    //std::cout << "ref cnt " << instance.use_count() << std::endl;
    environment->define("this", instance);
    std::shared_ptr<LoxFunction> func =  std::make_shared<LoxFunction>(declaration, environment);
    return func;
}
