#pragma once

#include "ast.h"

struct AstPrinter : public ast::AstVisitor<std::string> {
    std::string VisitUnaryExpr(ast::Unary &) override;
    std::string VisitStringExpr(ast::String &) override;
    std::string VisitNumberExpr(ast::Number &) override;
    std::string VisitBoolExpr(ast::Bool &) override;
    std::string VisitBinaryExpr(ast::Binary &) override;
    std::string VisitGroupExpr(ast::Group &) override;
    std::string VisitNilExpr(ast::Nil&) override;

    static void Print(ast::Expr& expr);
};