
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
struct Nil;
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
	virtual R visit_nil_expr(Nil&) = 0;
};

struct Expr
{
	virtual ~Expr() = default;
	virtual void accept(AstVisitor<void>& visitor) = 0;
	virtual std::string accept(AstVisitor<std::string>& visitor) = 0;
};

struct Unary : public Expr
{
	TokenType op;
	std::unique_ptr<Expr> value;
	Unary(TokenType op_, std::unique_ptr<Expr> value_) :op(op_), value(std::move(value_)) {}
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
	std::string_view value;
	String(std::string_view value_) :value(value_) {}
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
	Number(double value_) :value(value_) {}
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
	Bool(bool value_) :value(value_) {}
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
	std::unique_ptr<Expr> left;
	TokenType op;
	std::unique_ptr<Expr> right;
	Binary(std::unique_ptr<Expr> left_, TokenType op_, std::unique_ptr<Expr> right_) :left(std::move(left_)), op(op_), right(std::move(right_)) {}
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
	Group(std::unique_ptr<Expr> expr_) :expr(std::move(expr_)) {}
	void accept(AstVisitor<void>& visitor) override
	{
		return visitor.visit_group_expr(*this);
	}
	std::string accept(AstVisitor<std::string>& visitor) override
	{
		return visitor.visit_group_expr(*this);
	}
};

struct Nil : public Expr
{
	Nil() {}
	void accept(AstVisitor<void>& visitor) override
	{
		return visitor.visit_nil_expr(*this);
	}
	std::string accept(AstVisitor<std::string>& visitor) override
	{
		return visitor.visit_nil_expr(*this);
	}
};

