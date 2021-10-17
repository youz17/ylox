#pragma once

#include "expr.h"

struct AstPrinter : public expr::Visitor<std::string> {
    std::string VisitUnary(expr::Unary &) override;
    std::string VisitString(expr::String &) override;
    std::string VisitNumber(expr::Number &) override;
    std::string VisitBool(expr::Bool &) override;
    std::string VisitBinary(expr::Binary &) override;
    std::string VisitGroup(expr::Group &) override;
    std::string VisitNil(expr::Nil&) override;
    std::string VisitPrint(expr::Print&) override;
    std::string VisitExpression(expr::Expression&) override;

    static void Print(expr::Expr& expr);
};