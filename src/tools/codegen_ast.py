from typing import Dict, List, Tuple

asts: Dict[str, List[Tuple[str, str]]] = {
    "Unary": [("TokenType", "op"), ("Box<Expr>", "value")],
    "String": [("std::string", "value")],
    "Number": [("double", "value")],
    "Bool": [("bool", "value")],
    "Binary": [("TokenType", "op"), ("Box<Expr>", "left"), ("Box<Expr>", "right")],
    "Group": [("Box<Expr>", "expr")]
}

ast_visitor_types: List[str] = ["void", "std::string"]

header: str = """
#include <string>
#include "box.h"
#include "scanner.h"
"""


def gen_visitor_code():
    print('template<typename R>')
    print('struct AstVisitor{')
    for [type_name, _] in asts.items():
        print(f'virtual R visit_{type_name.lower()}_expr({type_name}&) = 0;')
    print('};')


def gen_visitor_accept(name: str):
    for ret in ast_visitor_types:
        print(f'{ret} accept(AstVisitor<{ret}>& visitor) override {{')
        print(f'    return visitor.visit_{name.lower()}_expr(*this);')
        print('}')


def gen_expr():
    print('struct Expr{')
    for ret in ast_visitor_types:
        print(f'virtual {ret} accept(AstVisitor<{ret}>& visitor);')
    print('};')
    for [name, fields] in asts.items():
        print(f'struct {name} : public Expr {{')
        for [f_type, f_name] in fields:
            print(f'{f_type} {f_name};')
        print()
        print(f'bool operator<=>(const {name}&) const = default;')
        gen_visitor_accept(name)
        print('};')
        print()


def gen_ast_expr():
    for [type_name, _] in asts.items():
        print(f'struct {type_name};')

    gen_visitor_code()
    print()
    gen_expr()
    pass


def main():
    print(header)
    gen_ast_expr()


if __name__ == '__main__':
    main()
