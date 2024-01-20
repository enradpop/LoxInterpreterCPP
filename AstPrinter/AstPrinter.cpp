#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "../Expr.h"
//TODO Should be templated on return type?
class AstPrinter : public Visitor<std::string> {
public:
    std::string parenthesize(std::string name, std::vector<Expr<std::string>*> exprs) {
        std::ostringstream builder;
        builder << "(" << name;
        for(auto e : exprs) {
            builder << " " << e->accept(this);
        }
        builder << ")";
        return builder.str();
    }

    std::string print(Expr<std::string>* expr) {
        return expr->accept(this);
    }

    std::string visitBinaryExpr(Binary<std::string>* expr) override {
        std::vector<Expr<std::string>*> vect;
        vect.push_back(expr->left);vect.push_back(expr->right);
        return parenthesize(expr->oprtr->_lexeme, vect);
    }

    std::string visitGroupingExpr(Grouping<std::string>* expr) override {
        std::vector<Expr<std::string>*> vect;
        vect.push_back(expr->expression);
        return parenthesize("group", vect);
    }

    std::string visitLiteralExpr(Literal<std::string>* expr) override { 
        if(expr->value->_type == Token::STRING)
            return std::get<std::string>(*(expr->value->_literal));
        else if (expr->value->_type == Token::NUMBER)
            return  std::to_string(std::get<double>(*(expr->value->_literal)));
        else return "nil";//TODO exc?
    }

    std::string visitUnaryExpr(Unary<std::string>* expr) override {
        std::vector<Expr<std::string>*> vect;
        vect.push_back(expr->right);
        return parenthesize(expr->oprtr->_lexeme, vect);
    }
};

int main() {
    //(* (- 123) (group 45.67))
    Expr<std::string>* expression = new Binary<std::string>(
        new Unary<std::string>(
            new Token(Token::MINUS, "-", std::nullopt, 1),
            new Literal<std::string>(new Token(Token::NUMBER, "123", 123.0, 1))),
        new Token(Token::STAR, "*", std::nullopt, 1),
        new Grouping<std::string>(
            new Literal<std::string>(new Token(Token::NUMBER, "45", 45.0, 1))));

    AstPrinter* printer = new AstPrinter;
    std::cout << printer->print(expression);
}