#pragma once

#include <variant>
#include <string>

#include "expr.h"
#include "literal.h"

class Interpreter :public expr::Visitor<Literal>
{
public:
	Literal VisitBinary(expr::Binary& bin) override;
	Literal VisitBool(expr::Bool& b) override;
	Literal VisitGroup(expr::Group& g) override;
	Literal VisitNil(expr::Nil& nil) override;
	Literal VisitNumber(expr::Number& num) override;
	Literal VisitString(expr::String& str) override;
	Literal VisitUnary(expr::Unary& unary) override;
	Literal VisitPrint(expr::Print&) override;
	Literal VisitExpression(expr::Expression&) override;

	static Literal Eval(expr::Expr& expr);

private:
	Literal _Eval(unique_ptr<expr::Expr>& expr);
};
