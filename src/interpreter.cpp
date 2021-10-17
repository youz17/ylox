#include "stdafx.h"

#include "interpreter.h"
#include "tools.h"
#include "error.h"

using tool::variant_index;

Literal Interpreter::VisitBinary(expr::Binary& bin)
{
	const auto left = _Eval(bin.left);
	const auto right = _Eval(bin.right);
	try
	{
		switch (bin.op.type)
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
			if (std::get<double>(right) == 0)
				throw InterpreterError(bin.op, "/0 error");
			return std::get<double>(left) / std::get<double>(right);
		case TokenType::Star:
			return std::get<double>(left) * std::get<double>(right);
		case TokenType::Greater:
			return std::get<double>(left) > std::get<double>(right);
		case TokenType::GreaterEqual:
			return std::get<double>(left) >= std::get<double>(right);
		case TokenType::Less:
			return std::get<double>(left) < std::get<double>(right);
		case TokenType::LessEqual:
			return std::get<double>(left) <= std::get<double>(right);
		case TokenType::EqualEqual:
			if (left.index() != right.index())
				throw InterpreterError(bin.op, "== left type not same as right");
			return left == right;
		default:
			throw InterpreterError(bin.op, "not implement bin op");
		}
	}
	catch (std::bad_variant_access&)
	{
		throw InterpreterError(bin.op, "error binary type");
	}
}

Literal Interpreter::VisitBool(expr::Bool& b)
{
	return b.value;
}

Literal Interpreter::VisitGroup(expr::Group& g)
{
	return _Eval(g.expr);
}

Literal Interpreter::VisitNil(expr::Nil& nil)
{
	return nullptr;
}

Literal Interpreter::VisitNumber(expr::Number& num)
{
	return num.value;
}

Literal Interpreter::VisitString(expr::String& str)
{
	return std::string(str.value);
}

Literal Interpreter::VisitUnary(expr::Unary& unary)
{
	const auto value = _Eval(unary.value);
	try
	{
		switch (unary.op.type)
		{
		case TokenType::Bang:
			return !(std::get<bool>(value));
		case TokenType::Minus:
			return -(std::get<double>(value));
		default:
			throw InterpreterError(unary.op, "error unary op");
		}
	}
	catch (std::bad_variant_access&)
	{
		throw InterpreterError(unary.op, "error unary type");
	}
}

Literal Interpreter::Eval(expr::Expr& expr)
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

Literal Interpreter::_Eval(unique_ptr<expr::Expr>& expr)
{
	return expr->Accept(*this);
}
