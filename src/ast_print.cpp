#include "stdafx.h"

#include "ast_print.h"

using namespace ast;

std::string AstPrinter::VisitUnaryExpr(Unary &unary) {
    return std::format("({} {})", unary.op, unary.value->Accept(*this));
}

std::string AstPrinter::VisitStringExpr(String &str) { return std::string(str.value); }

std::string AstPrinter::VisitNumberExpr(Number &num) {
    return std::format("{}", num.value);
}

std::string AstPrinter::VisitBoolExpr(Bool &b) {
    return b.value ? "true" : "false";
}

std::string AstPrinter::VisitBinaryExpr(Binary &bin) {
    return std::format("({} {} {})", bin.op, bin.left->Accept(*this),
                       bin.right->Accept(*this));
}

std::string AstPrinter::VisitGroupExpr(Group &group) {
    return std::format("(group {})", group.expr->Accept(*this));
}

std::string AstPrinter::VisitNilExpr(Nil&)
{
    return "nil";
}

void AstPrinter::Print(Expr &expr) {
    AstPrinter p;
    std::cout << expr.Accept(p) << std::endl;
}
