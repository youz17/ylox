#pragma once

#include <variant>
#include <string>

#include "ast.h"
#include "literal.h"

class Interpreter :public ast::AstVisitor<Literal>
{
public:
	Literal VisitBinaryExpr(ast::Binary& bin) override;
	Literal VisitBoolExpr(ast::Bool& b) override;
	Literal VisitGroupExpr(ast::Group& g) override;
	Literal VisitNilExpr(ast::Nil& nil) override;
	Literal VisitNumberExpr(ast::Number& num) override;
	Literal VisitStringExpr(ast::String& str) override;
	Literal VisitUnaryExpr(ast::Unary& unary) override;

	static Literal Eval(ast::Expr& expr);

private:
	Literal _Eval(unique_ptr<ast::Expr>& expr);
};
