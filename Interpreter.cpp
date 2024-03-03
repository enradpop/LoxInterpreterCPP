#include "Interpreter.h"
#include "Lox.h"

#include <vector>

ReturnType Interpreter::visitGroupingExpr(Grouping<ReturnType>& expr) {
    return evaluate(*expr.expression);
}
ReturnType Interpreter::evaluate(Expr<ReturnType>& expr) {
    return expr.accept(*this);
}

ReturnType Interpreter::visitBinaryExpr(Binary<ReturnType>& expr) {
    ReturnType left = evaluate(*expr.left);
    ReturnType right = evaluate(*expr.right);
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
        return std::get<double>(left) > std::get<double>(right);
    case Token::LESS:
        checkNumberOperands(expr.oprtr, left, right);
        return std::get<double>(left) < std::get<double>(right);
    case Token::LESS_EQUAL:
        checkNumberOperands(expr.oprtr, left, right);
        return std::get<double>(left) < std::get<double>(right);
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
ReturnType Interpreter::visitLiteralExpr(Literal<ReturnType>& expr) {
    if(std::holds_alternative<double>(*expr.value._literal)) {
        return std::get<double>(*expr.value._literal);
    }
    if(std::holds_alternative<bool>(*expr.value._literal)) {
        return std::get<bool>(*expr.value._literal);
    }
    else {
        return std::get<std::string>(*expr.value._literal);
    }
}

ReturnType Interpreter::visitUnaryExpr(Unary<ReturnType>& expr) {
    ReturnType right =  evaluate(*expr.right);
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

ReturnType Interpreter::visitVariableExpr(Variable<ReturnType>& expr) {
    return _environment.get(expr.name);
}

ReturnType Interpreter::visitAssignExpr(Assign<ReturnType>& expr) {
    ReturnType value = evaluate(*expr.value);
    _environment.assign(expr.name, value);
    return value;
}

void Interpreter::visitExpressionStmt(ExpressionStmt<ReturnType>& exprStmt) {
    evaluate(*exprStmt.expression);
}

void Interpreter::visitPrintStmt(Print<ReturnType>& printStmt) {
    ReturnType value = evaluate(*printStmt.expression);
    std::cout << stringify(value) <<std::endl;
}

void Interpreter::visitVarStmt(Var<ReturnType>& var) {
    ReturnType value = nullptr;
    if(var.initializer != nullptr) {
        value = evaluate(*var.initializer);
    }
    _environment.define(var.name._lexeme, value);
}

void Interpreter::checkNumberOperand(Token oprtr, ReturnType& operand) {
    if(std::holds_alternative<double>(operand)) return;
    throw RuntimeError(oprtr, "Operand must be a number");
}

void Interpreter::checkNumberOperands(Token oprtr, ReturnType& left, ReturnType& right) {
    if(std::holds_alternative<double>(left)
    && std::holds_alternative<double>(right)) return;
    throw RuntimeError(oprtr, "Operands must be a numbers");
}


void Interpreter::interpret(std::vector<Stmt<ReturnType>*> const& statements) {
    try {
        for(auto& s : statements) {
            execute(*s);
        }
    }
    catch (RuntimeError& re) {
        Lox::runtimeError(re);
    }
}

void Interpreter::execute(Stmt<ReturnType>& stmt) {
    stmt.accept(*this);
}

bool Interpreter::isTruthy(ReturnType const& object){
    if(std::holds_alternative<bool>(object)) {
        return std::get<bool>(object);
    }
    else return true;
    //TODO nil?
}

bool Interpreter::isEqual(ReturnType const& left, ReturnType const& right) {
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

std::string Interpreter::stringify(ReturnType const& object) {
    if(std::holds_alternative<bool>(object)) {
        return std::to_string(std::get<bool>(object));
    }
    else if(std::holds_alternative<double>(object)) {
        return std::to_string(std::get<double>(object));
    }
    else if(std::holds_alternative<std::string>(object)) {
        return std::get<std::string>(object);
    }
    else return "null";

}
