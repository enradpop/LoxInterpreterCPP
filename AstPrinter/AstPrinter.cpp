#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "../Expr.h"
//TODO replace dumb repetition of type
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

    //TODO, really bad, java uses Object, for both string and numerical literals
    //have to build Literal:Expr in a different way
    std::string visitLiteralExpr(Literal<std::string>* expr) override { 
        //if (expr->value == NULL) return "nil";
        return std::to_string(expr->value);
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
            new Token(Token::MINUS, "-", NULL, 1),
            new Literal<std::string>(123)),
        new Token(Token::STAR, "*", NULL, 1),
        new Grouping<std::string>(
            new Literal<std::string>(45)));

    AstPrinter* printer = new AstPrinter;
    std::cout << printer->print(expression);

    

    // Expr expression = new Expr.Binary(
    //     new Expr.Unary(
    //         new Token(TokenType.MINUS, "-", null, 1),
    //         new Expr.Literal(123)),
    //     new Token(TokenType.STAR, "*", null, 1),
    //     new Expr.Grouping(
    //         new Expr.Literal(45.67)));
}