#pragma once

#include <vector>

#include "scanner.h"
#include "expr.h"

// 输入不应该是一个只有 EOF 或者是 empty 的 tokens
unique_ptr<expr::Expr> Parse(const vector<Token>& tokens);
