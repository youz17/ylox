#pragma once

#include "ast.h"

struct AstPrinter : public AstVisitor<std::string> {
    std::string visit_unary_expr(Unary &) override;
    std::string visit_string_expr(String &) override;
    std::string visit_number_expr(Number &) override;
    std::string visit_bool_expr(Bool &) override;
    std::string visit_binary_expr(Binary &) override;
    std::string visit_group_expr(Group &) override;
    std::string visit_nil_expr(Nil&) override;

    static void Print(Expr& expr);
};