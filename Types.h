#pragma once
#include "Token.h"
#include <variant>
#include <memory>
#include <variant>
#include <stdexcept>
#include <vector>

class LoxCallable;
class LoxClass;
class LoxInstance;

class RuntimeError : public std::runtime_error {
public:
    RuntimeError(Token token, std::string const& message) : std::runtime_error(message.c_str()), _token(token)
    {}
    Token _token;
};

using CallableObject = std::shared_ptr<LoxCallable>;
using LoxInstanceValue = std::shared_ptr<LoxInstance>;
using ExpressionValue = std::variant<double, std::string, bool, std::nullptr_t, CallableObject, LoxInstanceValue>;

class Return: public std::runtime_error {
public:
    Return(ExpressionValue const &value) : std::runtime_error(""), value(value) {}
    ExpressionValue value;
};