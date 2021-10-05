#pragma once

#include "ast.h"

struct AstPrinter : public AstVisitor<std::string> {
    std::string VisitUnaryExpr(Unary &) override;
    std::string VisitStringExpr(String &) override;
    std::string VisitNumberExpr(Number &) override;
    std::string VisitBoolExpr(Bool &) override;
    std::string VisitBinaryExpr(Binary &) override;
    std::string VisitGroupExpr(Group &) override;
    std::string VisitNilExpr(Nil&) override;

    static void Print(Expr& expr);
};