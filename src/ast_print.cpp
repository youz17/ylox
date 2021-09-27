#include "stdafx.h"

#include "ast_print.h"

std::string AstPrinter::visit_unary_expr(Unary &unary) {
    return std::format("({} {})", unary.op, unary.value->accept(*this));
}

std::string AstPrinter::visit_string_expr(String &str) { return str.value; }

std::string AstPrinter::visit_number_expr(Number &num) {
    return std::format("{}", num.value);
}

std::string AstPrinter::visit_bool_expr(Bool &b) {
    return b.value ? "true" : "false";
}

std::string AstPrinter::visit_binary_expr(Binary &bin) {
    return std::format("({} {} {})", bin.op, bin.left->accept(*this),
                       bin.right->accept(*this));
}

std::string AstPrinter::visit_group_expr(Group &group) {
    return std::format("(group {})", group.expr->accept(*this));
}

void AstPrinter::print(Expr &expr) {
    AstPrinter p;
    std::cout << expr.accept(p) << std::endl;
}
