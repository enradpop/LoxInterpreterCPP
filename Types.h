#pragma once
#include "Token.h"
#include <variant>

class LoxCallable;

class RuntimeError : public std::runtime_error {
public:
    RuntimeError(Token token, std::string const& message) : std::runtime_error(message.c_str()), _token(token)
    {}
    Token _token;
};

using FunctionObject = std::shared_ptr<LoxCallable>;
using ExpressionValue = std::variant<double, std::string, bool, std::nullptr_t, FunctionObject>;

class Return: public std::runtime_error {
public:
    Return(ExpressionValue const &value) : std::runtime_error(""), value(value) {}
    ExpressionValue value;
};