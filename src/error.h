#pragma once

#include <stdexcept>

#include "scanner.h"
#include "expr.h"

class ScanError : public std::runtime_error
{
public:
	ScanError(size_t line, char c, const std::string& message) : std::runtime_error(message), m_line(line), m_char(c) {}
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
	explicit ParseError(const Token& token, const std::string& message) : std::runtime_error(message), m_token(token) {}
	explicit ParseError(const Token& token) : ParseError(token, "parse error") {}

private:
	Token m_token;
};

class InterpreterError : public std::runtime_error
{
public:
	InterpreterError(const Token& token, const std::string& message) : std::runtime_error(message), m_token(token) {}
	InterpreterError(const std::string& message) : std::runtime_error(message), m_token(TokenType::Eof, -1) {}

	Token& GetToken() { return m_token; }

private:
	Token m_token;
};
