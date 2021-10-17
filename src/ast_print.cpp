#include "stdafx.h"

#include "ast_print.h"

using namespace expr;

std::string AstPrinter::VisitUnary(Unary& unary) {
	return std::format("({} {})", unary.op, unary.value->Accept(*this));
}

std::string AstPrinter::VisitString(String& str) { return std::string(str.value); }

std::string AstPrinter::VisitNumber(Number& num) {
	return std::format("{}", num.value);
}

std::string AstPrinter::VisitBool(Bool& b) {
	return b.value ? "true" : "false";
}

std::string AstPrinter::VisitBinary(Binary& bin) {
	return std::format("({} {} {})", bin.op, bin.left->Accept(*this),
		bin.right->Accept(*this));
}

std::string AstPrinter::VisitGroup(Group& group) {
	return std::format("(group {})", group.expr->Accept(*this));
}

std::string AstPrinter::VisitNil(Nil&)
{
	return "nil";
}

void AstPrinter::Print(Expr& expr) {
	AstPrinter p;
	std::cout << expr.Accept(p) << std::endl;
}
