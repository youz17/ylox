#include "stdafx.h"

#include "interpreter.h"
#include "tools.h"
#include "error.h"

using tool::variant_index;

Literal Interpreter::VisitBinaryExpr(ast::Binary& bin)
{
	const auto left = _Eval(bin.left);
	const auto right = _Eval(bin.right);
	switch (bin.op)
	{
	case TokenType::And:
		return std::get<bool>(left) && std::get<bool>(right);
	case TokenType::Or:
		return std::get<bool>(left) || std::get<bool>(right);
	case TokenType::Plus:
	{
		if (left.index() == variant_index<Literal, double>())
			return std::get<double>(left) + std::get<double>(right);
		if (left.index() == variant_index<Literal, std::string>())
			return std::get<std::string>(left) + std::get<std::string>(right);
		throw InterpreterError("error type, not support add");
	}
	case TokenType::Minus:
		return std::get<double>(left) - std::get<double>(right);
	case TokenType::Slash:
		return std::get<double>(left) / std::get<double>(right);
	case TokenType::Star:
		return std::get<double>(left) * std::get<double>(right);
	case TokenType::EqualEqual:
		if (left.index() != right.index())
			throw InterpreterError("== left type not same as right");
		return left == right;
	default:
		throw InterpreterError("not implement bin op");
	}
}

Literal Interpreter::VisitBoolExpr(ast::Bool& b)
{
	return b.value;
}

Literal Interpreter::VisitGroupExpr(ast::Group& g)
{
	return _Eval(g.expr);
}

Literal Interpreter::VisitNilExpr(ast::Nil& nil)
{
	return nullptr;
}

Literal Interpreter::VisitNumberExpr(ast::Number& num)
{
	return num.value;
}

Literal Interpreter::VisitStringExpr(ast::String& str)
{
	return std::string(str.value);
}

Literal Interpreter::VisitUnaryExpr(ast::Unary& unary)
{
	const auto value = _Eval(unary.value);
	switch (unary.op)
	{
	case TokenType::Bang:
		return !(std::get<bool>(value));
	case TokenType::Minus:
		return -(std::get<double>(value));
	default:
		throw InterpreterError("error unary op");
	}
}

Literal Interpreter::Eval(ast::Expr& expr)
{
	try
	{
		Interpreter i;
		return expr.Accept(i);
	}
	catch (std::bad_variant_access&)
	{
		throw InterpreterError("error type");
	}
}

Literal Interpreter::_Eval(unique_ptr<ast::Expr>& expr)
{
	return expr->Accept(*this);
}
