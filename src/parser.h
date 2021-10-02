#pragma once

#include <utility>
#include <vector>

#include "scanner.h"
#include "ast.h"

unique_ptr<Expr> Parse(const vector<Token>& tokens);
