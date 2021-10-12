#pragma once

#include <vector>

#include "scanner.h"
#include "ast.h"

// 输入不应该是一个只有 EOF 或者是 empty 的 tokens
unique_ptr<ast::Expr> Parse(const vector<Token>& tokens);
