#pragma once

#include <vector>

#include "scanner.h"
#include "expr.h"
#include "stmt.h"

// ���벻Ӧ����һ��ֻ�� EOF ������ empty �� tokens
using StmtVec =vector<unique_ptr<stmt::Stmt>>; 
StmtVec Parse(const vector<Token>& tokens);
