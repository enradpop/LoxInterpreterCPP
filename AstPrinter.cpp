#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "Expr.h"
#include <cassert>
#include "AstPrinter.h"

// bool g_enableLogging = false;

//TODO Should be templated on return type?
std::string AstPrinter::parenthesize(std::string name, std::vector<std::unique_ptr<Expr<std::string>>>& exprs) {
    std::ostringstream builder;
    builder << "(" << name;
    for(auto& e : exprs) {
        builder << " " << e->accept(*this);
    }
    builder << ")";
    return builder.str();
}

std::string AstPrinter::print(Expr<std::string>* expr) {
    return expr->accept(*this);
}

std::string AstPrinter::visitBinaryExpr(Binary<std::string>& expr) {
    std::vector<std::unique_ptr<Expr<std::string>>> vect;
    vect.push_back(std::move(expr.left));vect.push_back(std::move(expr.right));
    return parenthesize(expr.oprtr._lexeme, vect);
}

std::string AstPrinter::visitGroupingExpr(Grouping<std::string>& expr) {
    std::vector<std::unique_ptr<Expr<std::string>>> vect;
    vect.push_back(std::move(expr.expression));
    return parenthesize("group", vect);
}

std::string AstPrinter::visitLiteralExpr(Literal<std::string>& expr) { 
    if(expr.value._type == Token::STRING)
        return std::get<std::string>(*(expr.value._literal));
    else if (expr.value._type == Token::NUMBER)
        return  std::to_string(std::get<double>(*(expr.value._literal)));
    assert(true);
    return "nil";
}

std::string AstPrinter::visitUnaryExpr(Unary<std::string>& expr) {
    std::vector<std::unique_ptr<Expr<std::string>>> vect;
    vect.push_back(std::move(expr.right));
    return parenthesize(expr.oprtr._lexeme, vect);
}

// int main() {
//     //(* (- 123) (group 45.67))
//     Expr<std::string>* expression = new Binary<std::string>(
//         new Unary<std::string>(
//             {Token::MINUS, "-", std::nullopt, 1},
//             new Literal<std::string>({Token::NUMBER, "123", 123.0, 1})),
//         {Token::STAR, "*", std::nullopt, 1},
//         new Grouping<std::string>(
//             new Literal<std::string>({Token::NUMBER, "45", 45.0, 1})));

//     AstPrinter printer;
//     std::cout << printer.print(expression);
//     delete expression;
// }