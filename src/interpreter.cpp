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
			throw RuntimeError("error type, not support add");
		}
		case TokenType::Minus:
			return std::get<double>(left) - std::get<double>(right);
		case TokenType::Slash:
			if (std::get<double>(right) == 0)
				throw RuntimeError(bin.op, "/0 error");
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
				throw RuntimeError(bin.op, "== left type not same as right");
			return left == right;
		default:
			throw RuntimeError(bin.op, "not implement bin op");
		}
	}
	catch (std::bad_variant_access&)
	{
		throw RuntimeError(bin.op, "error binary type");
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
			throw RuntimeError(unary.op, "error unary op");
		}
	}
	catch (std::bad_variant_access&)
	{
		throw RuntimeError(unary.op, "error unary type");
	}
}

Literal Interpreter::VisitVariable(expr::Variable& var)
{
	return _Get(var.name);
}

void Interpreter::VisitExpression(stmt::Expression& e)
{
	_Eval(e.expr);
}

void Interpreter::VisitPrint(stmt::Print& p)
{
	std::cout << _Eval(p.expr) << std::endl;
}

void Interpreter::VisitLet(stmt::Let& l)
{
	// todo: ¼ì²éÊÇ·ñ´æÔÚ
	m_vars.emplace(l.name, _Eval(l.expr));
}

void Interpreter::Eval(const StmtVec& stmts)
{
	try
	{
		for (auto& stmt : stmts)
		{
			stmt->Accept(*this);
		}
	}
	catch (std::bad_variant_access&)
	{
		throw RuntimeError("error type");
	}
}

Literal Interpreter::_Eval(unique_ptr<expr::Expr>& expr)
{
	return expr->Accept(*this);
}

const Literal& Interpreter::_Get(std::string_view var_name) const
{
	const auto iter = m_vars.find(var_name);
	if (iter != m_vars.end())
		return  iter->second;
	throw RuntimeError(std::format("can not find var {}", var_name));
}
