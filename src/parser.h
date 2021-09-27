#pragma once

#include <utility>

#include "scanner.h"
#include "ast.h"

class Parser
{
public:
	explicit Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) { }

private:
	Expr& Expression();

private:
	const std::vector<Token> m_tokens;
	size_t m_idx = 0;
};
