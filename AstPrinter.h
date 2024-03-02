#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "Expr.h"
#include <cassert> 
class AstPrinter : public Visitor<std::string> {
public:
    std::string parenthesize(std::string name, std::vector<std::unique_ptr<Expr<std::string>>>& exprs) ;
    std::string print(Expr<std::string>* expr) ;
    std::string visitBinaryExpr(Binary<std::string>& expr) override;
    std::string visitGroupingExpr(Grouping<std::string>& expr) override;
    std::string visitLiteralExpr(Literal<std::string>& expr) override;
    std::string visitUnaryExpr(Unary<std::string>& expr) override;

};