#include "ast.h"

struct AstPrinter : public AstVisitor<std::string> {
    std::string visit_unary_expr(Unary &);
    std::string visit_string_expr(String &);
    std::string visit_number_expr(Number &);
    std::string visit_bool_expr(Bool &);
    std::string visit_binary_expr(Binary &);
    std::string visit_group_expr(Group &);

    static void print(Expr& expr);
};