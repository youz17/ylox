#pragma once

#include <vector>

#include "scanner.h"
#include "expr.h"

// ���벻Ӧ����һ��ֻ�� EOF ������ empty �� tokens
unique_ptr<expr::Expr> Parse(const vector<Token>& tokens);
