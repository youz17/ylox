#pragma once

#include <string>
#include <map>

#include "expr.h"
#include "stmt.h"
#include "literal.h"

using StmtVec =vector<unique_ptr<stmt::Stmt>>; 
class Interpreter :public expr::Visitor<Literal>, public stmt::Visitor<void>
{
public:
	Literal VisitBinary(expr::Binary& bin) override;
	Literal VisitBool(expr::Bool& b) override;
	Literal VisitGroup(expr::Group& g) override;
	Literal VisitNil(expr::Nil& nil) override;
	Literal VisitNumber(expr::Number& num) override;
	Literal VisitString(expr::String& str) override;
	Literal VisitUnary(expr::Unary& unary) override;
	Literal VisitVariable(expr::Variable& var) override;

	void VisitExpression(stmt::Expression& e) override;
	void VisitPrint(stmt::Print& p) override;
	void VisitLet(stmt::Let& l) override;

	void Eval(const StmtVec& expr);

private:
	Literal _Eval(unique_ptr<expr::Expr>& expr);
	const Literal& _Get(std::string_view var_name) const;

private:
	std::map<std::string, Literal, std::less<>> m_vars;
};
