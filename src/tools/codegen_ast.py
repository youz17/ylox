from typing import Dict, List, Tuple

asts: Dict[str, List[Tuple[str, str]]] = {
    "Unary": [("TokenType", "op"), ("std::unique_ptr<Expr>", "value")],
    "String": [("std::string_view", "value")],
    "Number": [("double", "value")],
    "Bool": [("bool", "value")],
    "Binary": [("std::unique_ptr<Expr>", "left"), ("TokenType", "op"), ("std::unique_ptr<Expr>", "right")],
    "Group": [("std::unique_ptr<Expr>", "expr")],
    "Nil": []
}

ast_visitor_types: List[str] = ["Literal", "std::string"]

header: str = """
#pragma once

#include <string>
#include <memory>
#include "scanner.h"
#include "literal.h"
namespace ast{
"""

ender: str = "}"


def gen_visitor_code():
    print('template<typename R>')
    print('struct AstVisitor{')
    print('virtual ~AstVisitor() = default;')
    for [type_name, _] in asts.items():
        print(f'virtual R Visit{type_name}Expr({type_name}&) = 0;')
    print('};')


def gen_visitor_accept(name: str):
    for ret in ast_visitor_types:
        print(f'{ret} Accept(AstVisitor<{ret}>& visitor) override {{')
        print(f'    return visitor.Visit{name}Expr(*this);')
        print('}')


def gen_constructor(name: str, fields: List[Tuple[str, str]]):
    def printnl(s): return print(s, end='')
    if (len(fields) == 1):
        printnl("explicit ")
    printnl(f'{name}(')

    if len(fields) == 0:
        printnl(') = default;')
        return

    printnl(f'{fields[0][0]} {fields[0][1]}_')
    for i in range(1, len(fields)):
        printnl(f', {fields[i][0]} {fields[i][1]}_')
    printnl('):')

    def helper(filed: Tuple[str, str]):
        if filed[0].startswith('std::unique_ptr') or filed[0] == 'std::string':
            return f'std::move({filed[1]}_)'
        return f'{filed[1]}_'
    printnl(f'{fields[0][1]}({helper(fields[0])})')
    for i in range(1, len(fields)):
        printnl(f', {fields[i][1]}({helper(fields[i])})')
    printnl('{}')


def gen_expr():
    print('struct Expr{')
    print('virtual ~Expr() = default;')
    for ret in ast_visitor_types:
        print(f'virtual {ret} Accept(AstVisitor<{ret}>& visitor) = 0;')
    print('};\n')
    for [name, fields] in asts.items():
        print(f'struct {name} final : public Expr {{')
        for [f_type, f_name] in fields:
            print(f'{f_type} {f_name};')
        gen_constructor(name, fields)
        print()
        # print(f'bool operator<=>(const {name}&) const = default;') # 这玩意干嘛的
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
    print(ender)


if __name__ == '__main__':
    main()
