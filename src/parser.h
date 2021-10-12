#pragma once

#include <vector>

#include "scanner.h"
#include "ast.h"

// ���벻Ӧ����һ��ֻ�� EOF ������ empty �� tokens
unique_ptr<ast::Expr> Parse(const vector<Token>& tokens);
