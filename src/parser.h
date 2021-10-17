#pragma once

#include <vector>

#include "scanner.h"
#include "expr.h"
#include "stmt.h"

// 输入不应该是一个只有 EOF 或者是 empty 的 tokens
using StmtVec =vector<unique_ptr<stmt::Stmt>>; 
StmtVec Parse(const vector<Token>& tokens);
