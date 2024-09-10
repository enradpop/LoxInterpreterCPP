#include "Interpreter.h"
#include "Lox.h"
#include "LoxFunction.h"
#include "LoxClass.h"
#include "LoxInstance.h"
#include <chrono>

#include <vector>

ExpressionValue Interpreter::visitGroupingExpr(Grouping<ExpressionValue>& expr) {
    return evaluate(*expr.expression);
}
ExpressionValue Interpreter::evaluate(Expr<ExpressionValue>& expr) {
    return expr.accept(*this);
}

ExpressionValue Interpreter::visitBinaryExpr(Binary<ExpressionValue>& expr) {
    ExpressionValue left = evaluate(*expr.left);
    ExpressionValue right = evaluate(*expr.right);
    switch (expr.oprtr._type)
    {
    case Token::MINUS:
        checkNumberOperands(expr.oprtr, left, right);
        return std::get<double>(left) - std::get<double>(right);
    case Token::SLASH:
        checkNumberOperands(expr.oprtr, left, right);
        return std::get<double>(left) / std::get<double>(right);
    case Token::STAR:
        checkNumberOperands(expr.oprtr, left, right);
        return std::get<double>(left) * std::get<double>(right);
    case Token::PLUS:
        if(std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
            return std::get<double>(left) + std::get<double>(right);
        }
        else if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
            return std::get<std::string>(left) + std::get<std::string>(right);
        }
        throw RuntimeError(expr.oprtr, "Operands must be two numbers or two strings.");
        break;
    case Token::GREATER:
        checkNumberOperands(expr.oprtr, left, right);
        return std::get<double>(left) > std::get<double>(right);
    case Token::GREATER_EQUAL:
        checkNumberOperands(expr.oprtr, left, right);
        return std::get<double>(left) >= std::get<double>(right);
    case Token::LESS:
        checkNumberOperands(expr.oprtr, left, right);
        return std::get<double>(left) < std::get<double>(right);
    case Token::LESS_EQUAL:
        checkNumberOperands(expr.oprtr, left, right);
        return std::get<double>(left) <= std::get<double>(right);
    case Token::BANG_EQUAL:
        return !isEqual(left, right);
    case Token::EQUAL_EQUAL:
        return isEqual(left, right);
    default:
        return nullptr;
        break;
    }
    return nullptr;
}
ExpressionValue Interpreter::visitLiteralExpr(Literal<ExpressionValue>& expr) {
    if(std::holds_alternative<double>(*expr.value._literal)) {
        return std::get<double>(*expr.value._literal);
    }
    if(std::holds_alternative<bool>(*expr.value._literal)) {
        return std::get<bool>(*expr.value._literal);
    }
    else if(std::holds_alternative<std::string>(*expr.value._literal)) {
        return std::get<std::string>(*expr.value._literal);
    }
    else return nullptr;
}

ExpressionValue Interpreter::visitLogicalExpr(Logical<ExpressionValue>& expr) {
    ExpressionValue left = evaluate(*expr.left);

    if (expr.oprtr._type == Token::OR) {
      if (isTruthy(left)) return left;
    } else {
      if (!isTruthy(left)) return left;
    }

    return evaluate(*expr.right);
}
ExpressionValue Interpreter::visitSetExpr(Set<ExpressionValue>& expr) {
    ExpressionValue object = evaluate(*expr.object);
    if(!std::holds_alternative<LoxInstanceValue>(object)) {
        throw RuntimeError(expr.name, "Only instances have fields.");
    }
    ExpressionValue value = evaluate(*expr.value);
    std::get<LoxInstanceValue>(object)->set(expr.name, value);
    return value;
}

ExpressionValue Interpreter::visitThisExpr(This<ExpressionValue>& expr) {
    return lookUpVariable(expr.keyword, expr);
}

ExpressionValue Interpreter::visitUnaryExpr(Unary<ExpressionValue>& expr) {
    ExpressionValue right =  evaluate(*expr.right);
    switch(expr.oprtr._type) {
        case Token::MINUS:
            checkNumberOperand(expr.oprtr, right);
            return -std::get<double>(right);
            break;
        case Token::BANG:
            return !isTruthy(right);
            break;
    }
    //unreachable 
    return nullptr;
}

ExpressionValue Interpreter::visitVariableExpr(Variable<ExpressionValue>& expr) {
    //return _environment->get(expr.name);
    return lookUpVariable(expr.name, expr);
}

ExpressionValue Interpreter::lookUpVariable(Token& name, Expr<ExpressionValue>& expr) {
    auto iter = _locals.find(&expr);
    if(iter != _locals.end()) {
        return _environment->getAt(iter->second, name._lexeme);
    }
    else {
        return _globals->get(name);
    }
}

ExpressionValue Interpreter::visitAssignExpr(Assign<ExpressionValue>& expr) {
    ExpressionValue value = evaluate(*expr.value);
    auto iter = _locals.find(&expr);
    if(iter != _locals.end()) {
        _environment->assignAt(iter->second, expr.name, value);
    }
    else {
        _globals->assign(expr.name, value);
    }
    return value;
}

ExpressionValue Interpreter::visitCallExpr(Call<ExpressionValue>& expr) {
    ExpressionValue callee = evaluate(*expr.callee);
    std::vector<ExpressionValue> arguments;
    for(auto& argument : expr.arguments)
    {
        arguments.emplace_back(evaluate(*argument));
    }
    if(!std::holds_alternative<CallableObject>(callee)) {
        throw RuntimeError(expr.paren, "Can only call functions and classes.");
    }
    CallableObject function =  std::get<CallableObject>(callee);
    if (arguments.size() != function->arity()) {
      throw RuntimeError(expr.paren, "Expected " + std::to_string(function->arity()) + " arguments but got " + std::to_string(arguments.size()) + ".");
    }
    return function->call(*this, arguments);
}

ExpressionValue Interpreter::visitGetExpr(Get<ExpressionValue>& expr) {
    ExpressionValue object = evaluate(*expr.object);
    if(std::holds_alternative<LoxInstanceValue>(object)) {
        LoxInstanceValue& instance = std::get<LoxInstanceValue>(object);
        return instance->get(expr.name, object);
    }
    throw RuntimeError(expr.name, "Only instances have properties.");
}

void Interpreter::visitExpressionStmt(ExpressionStmt<ExpressionValue>& exprStmt) {
    evaluate(*exprStmt.expression);
}

void Interpreter::visitFunctionStmt(Function<ExpressionValue>& stmt) {
    CallableObject function = std::make_shared<LoxFunction>(stmt, _environment);
    ExpressionValue func(function);
    _environment->define(stmt.name._lexeme, func);
}

void Interpreter::visitPrintStmt(Print<ExpressionValue>& printStmt) {
    ExpressionValue value = evaluate(*printStmt.expression);
    std::cout << stringify(value) <<std::endl;
}

void Interpreter::visitVarStmt(Var<ExpressionValue>& var) {
    ExpressionValue value = nullptr;
    if(var.initializer != nullptr) {
        value = evaluate(*var.initializer);
    }
    _environment->define(var.name._lexeme, value);
}

void Interpreter::visitReturnStmt(ReturnStmt<ExpressionValue> &returnStmt) {
    ExpressionValue value = nullptr;
    if(returnStmt.value != nullptr) {
        value = evaluate(*returnStmt.value);
    }
    throw Return(value);
}

void Interpreter::visitWhileStmt(While<ExpressionValue>& stmt) {
    while(isTruthy(evaluate(*stmt.condition))) {
        execute(*stmt.body);
    }
}

void Interpreter::visitBlockStmt(Block<ExpressionValue>& block) {
    auto newEnvironment = std::make_shared<Environment<ExpressionValue>>(_environment);
    executeBlock(block.statements, newEnvironment);
}

void Interpreter::visitClassStmt(Class<ExpressionValue>& stmt) {
    ExpressionValue nullValue = nullptr;
    _environment->define(stmt.name._lexeme, nullValue);
    std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods;
    for(auto& method : stmt.methods) {
        methods[method->name._lexeme] = std::make_shared<LoxFunction>(*method, _environment);
    }
    LoxClass* ptrKlass = new LoxClass(stmt.name._lexeme, methods);
    std::shared_ptr<LoxCallable> klass(ptrKlass);
    ExpressionValue klassValue(klass);
    _environment->assign(stmt.name, klassValue);
}

void Interpreter::visitIfStmt(If<ExpressionValue>& ifStmt) {
    if (isTruthy(evaluate(*ifStmt.condition))) {
      execute(*ifStmt.thenBranch);
    } else if (ifStmt.elseBranch != nullptr) {
      execute(*ifStmt.elseBranch);
    }
    return;
}

void Interpreter::checkNumberOperand(Token oprtr, ExpressionValue& operand) {
    if(std::holds_alternative<double>(operand)) return;
    throw RuntimeError(oprtr, "Operand must be a number");
}

void Interpreter::checkNumberOperands(Token oprtr, ExpressionValue& left, ExpressionValue& right) {
    if(std::holds_alternative<double>(left)
    && std::holds_alternative<double>(right)) return;
    throw RuntimeError(oprtr, "Operands must be a numbers");
}


void Interpreter::interpret(std::vector<std::unique_ptr<Stmt<ExpressionValue>>> const& statements) {
    try {
        for(auto& s : statements) {
            execute(*s);
        }
    }
    catch (RuntimeError& re) {
        Lox::runtimeError(re);
    }
}

void Interpreter::execute(Stmt<ExpressionValue>& stmt) {
    stmt.accept(*this);
}

void Interpreter::executeBlock(std::vector<std::unique_ptr<Stmt<ExpressionValue>>>& statements, std::shared_ptr<Environment<ExpressionValue>>& environment) {
    std::shared_ptr<Environment<ExpressionValue>> previous = _environment;
    try {
        _environment = environment;
        for(auto& s : statements) {
            execute(*s);
        }
    }
    catch(RuntimeError& re)
    {
        _environment = previous;
        throw re;
    }
    catch(Return& ret)
    {
        _environment = previous;
        throw ret;
    }
    _environment = previous;
}

bool Interpreter::isTruthy(ExpressionValue const& object){
    if(std::holds_alternative<bool>(object)) {
        return std::get<bool>(object);
    }
    else if (std::holds_alternative<std::nullptr_t>(object)) {
        return false;
    }
    else return true;
}

bool Interpreter::isEqual(ExpressionValue const& left, ExpressionValue const& right) {
    if(std::holds_alternative<nullptr_t>(left) && std::holds_alternative<nullptr_t>(right))
        return true;
    if(std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
        return std::get<double>(left) == std::get<double>(right);
    if(std::holds_alternative<bool>(left) && std::holds_alternative<bool>(right))
        return std::get<bool>(left) == std::get<bool>(right);
    if(std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right))
        return std::get<std::string>(left) == std::get<std::string>(right);
    return false;
}

std::string Interpreter::stringify(ExpressionValue const& object) {
    if(std::holds_alternative<bool>(object)) {
        return std::to_string(std::get<bool>(object));
    }
    else if(std::holds_alternative<double>(object)) {
        return std::to_string(std::get<double>(object));
    }
    else if(std::holds_alternative<std::string>(object)) {
        return std::get<std::string>(object);
    }
    else if(std::holds_alternative<CallableObject>(object)) {
        return std::get<CallableObject>(object)->toString();
    }
    else if(std::holds_alternative<LoxInstanceValue>(object)) {
        return std::get<LoxInstanceValue>(object)->toString();
    }
    else return "null";

}

void Interpreter::resolve(Expr<ExpressionValue>& expr, int depth) {
    _locals[&expr] = depth;
}

int Clock::arity() {
    return 0;
}

ExpressionValue Clock::call(Interpreter& interpreter, std::vector<ExpressionValue>& /*arguments*/)
{
    auto now = std::chrono::system_clock::now();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    return static_cast<double>(millis); 
}

std::string Clock::toString() {
    return "Clock native fn";
}
