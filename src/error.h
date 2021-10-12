#pragma once

#include <stdexcept>

#include "scanner.h"
#include "ast.h"

class ScanError : public std::runtime_error
{
public:
	ScanError(size_t line, char c, const char* message) : std::runtime_error(message), m_line(line), m_char(c) {}
	ScanError(size_t line, char c) : std::runtime_error("scan error"), m_line(line), m_char(c) {}

	size_t Line() const { return m_line; }
	char Char() const { return m_char; }

private:
	size_t m_line;
	char m_char;
};

class ParseError : public std::runtime_error
{
public:
	explicit ParseError(const Token& token, const char* message) : std::runtime_error(message), m_line(token.line), m_type(token.type), m_value(token.value) {}
	explicit ParseError(const Token& token) : ParseError(token, "parse error") {}

	TokenType GetTokenType() const { return m_type; }
	size_t Line() const { return m_line; }
	const std::string& Value() { return m_value; }

private:
	size_t m_line;
	TokenType m_type;
	std::string m_value;
};

class InterpreterError: public std::runtime_error
{
public:
	explicit InterpreterError(const char* message) : std::runtime_error(message), m_expr(nullptr){}

	const ast::Expr* GetExpr() const { return m_expr.get(); }

private:
	unique_ptr<ast::Expr> m_expr;
};
