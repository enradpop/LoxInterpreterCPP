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

using ReturnType = std::variant<double, std::string, bool, std::nullptr_t, LoxCallable*>;