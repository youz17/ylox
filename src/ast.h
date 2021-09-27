#pragma once

#include <string>
#include <memory>
#include "scanner.h"

struct Unary;
struct String;
struct Number;
struct Bool;
struct Binary;
struct Group;
template<typename R>
struct AstVisitor
{
	virtual ~AstVisitor() = default;
	virtual R visit_unary_expr(Unary&) = 0;
	virtual R visit_string_expr(String&) = 0;
	virtual R visit_number_expr(Number&) = 0;
	virtual R visit_bool_expr(Bool&) = 0;
	virtual R visit_binary_expr(Binary&) = 0;
	virtual R visit_group_expr(Group&) = 0;
};

struct Expr
{
	virtual ~Expr() = default;
	virtual void accept(AstVisitor<void>& visitor);
	virtual std::string accept(AstVisitor<std::string>& visitor);
};

struct Unary : public Expr
{
	TokenType op;
	std::unique_ptr<Expr> value;

	void accept(AstVisitor<void>& visitor) override
	{
		return visitor.visit_unary_expr(*this);
	}
	std::string accept(AstVisitor<std::string>& visitor) override
	{
		return visitor.visit_unary_expr(*this);
	}
};

struct String : public Expr
{
	std::string value;

	void accept(AstVisitor<void>& visitor) override
	{
		return visitor.visit_string_expr(*this);
	}
	std::string accept(AstVisitor<std::string>& visitor) override
	{
		return visitor.visit_string_expr(*this);
	}
};

struct Number : public Expr
{
	double value;

	void accept(AstVisitor<void>& visitor) override
	{
		return visitor.visit_number_expr(*this);
	}
	std::string accept(AstVisitor<std::string>& visitor) override
	{
		return visitor.visit_number_expr(*this);
	}
};

struct Bool : public Expr
{
	bool value;

	void accept(AstVisitor<void>& visitor) override
	{
		return visitor.visit_bool_expr(*this);
	}
	std::string accept(AstVisitor<std::string>& visitor) override
	{
		return visitor.visit_bool_expr(*this);
	}
};

struct Binary : public Expr
{
	TokenType op;
	std::unique_ptr<Expr> left;
	std::unique_ptr<Expr> right;

	void accept(AstVisitor<void>& visitor) override
	{
		return visitor.visit_binary_expr(*this);
	}
	std::string accept(AstVisitor<std::string>& visitor) override
	{
		return visitor.visit_binary_expr(*this);
	}
};

struct Group : public Expr
{
	std::unique_ptr<Expr> expr;

	void accept(AstVisitor<void>& visitor) override
	{
		return visitor.visit_group_expr(*this);
	}
	std::string accept(AstVisitor<std::string>& visitor) override
	{
		return visitor.visit_group_expr(*this);
	}
};

