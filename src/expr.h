#pragma once

#include <string>
#include <memory>

#include "expr.h"
#include "scanner.h"
#include "literal.h"

namespace expr
{
	struct Binary;
	struct Bool;
	struct Group;
	struct Nil;
	struct Number;
	struct String;
	struct Unary;
	struct Variable;

	template<typename R>
	struct Visitor
	{
		virtual ~Visitor() = default;

		virtual R VisitBinary(Binary&) = 0;
		virtual R VisitBool(Bool&) = 0;
		virtual R VisitGroup(Group&) = 0;
		virtual R VisitNil(Nil&) = 0;
		virtual R VisitNumber(Number&) = 0;
		virtual R VisitString(String&) = 0;
		virtual R VisitUnary(Unary&) = 0;
		virtual R VisitVariable(Variable&) = 0;
	};

	struct Expr
	{
		virtual ~Expr() = default;

		virtual Literal Accept(Visitor<Literal>&) = 0;
		virtual std::string Accept(Visitor<std::string>&) = 0;
	};

	struct Binary : public Expr
	{
		std::unique_ptr<Expr> left;
		Token op;
		std::unique_ptr<Expr> right;
		explicit Binary(std::unique_ptr<Expr> left_, Token op_, std::unique_ptr<Expr> right_) noexcept :left(std::move(left_)), op(op_), right(std::move(right_)) {}
		Literal Accept(Visitor<Literal>& visitor) override {
			return visitor.VisitBinary(*this);
		}
		std::string Accept(Visitor<std::string>& visitor) override {
			return visitor.VisitBinary(*this);
		}

	};

	struct Bool : public Expr
	{
		bool value;
		explicit Bool(bool value_) noexcept :value(value_) {}
		Literal Accept(Visitor<Literal>& visitor) override {
			return visitor.VisitBool(*this);
		}
		std::string Accept(Visitor<std::string>& visitor) override {
			return visitor.VisitBool(*this);
		}

	};

	struct Group : public Expr
	{
		std::unique_ptr<Expr> expr;
		explicit Group(std::unique_ptr<Expr> expr_) noexcept :expr(std::move(expr_)) {}
		Literal Accept(Visitor<Literal>& visitor) override {
			return visitor.VisitGroup(*this);
		}
		std::string Accept(Visitor<std::string>& visitor) override {
			return visitor.VisitGroup(*this);
		}

	};

	struct Nil : public Expr
	{
		explicit Nil() noexcept {}
		Literal Accept(Visitor<Literal>& visitor) override {
			return visitor.VisitNil(*this);
		}
		std::string Accept(Visitor<std::string>& visitor) override {
			return visitor.VisitNil(*this);
		}

	};

	struct Number : public Expr
	{
		double value;
		explicit Number(double value_) noexcept :value(value_) {}
		Literal Accept(Visitor<Literal>& visitor) override {
			return visitor.VisitNumber(*this);
		}
		std::string Accept(Visitor<std::string>& visitor) override {
			return visitor.VisitNumber(*this);
		}

	};

	struct String : public Expr
	{
		std::string_view value;
		explicit String(std::string_view value_) noexcept :value(value_) {}
		Literal Accept(Visitor<Literal>& visitor) override {
			return visitor.VisitString(*this);
		}
		std::string Accept(Visitor<std::string>& visitor) override {
			return visitor.VisitString(*this);
		}

	};

	struct Unary : public Expr
	{
		Token op;
		std::unique_ptr<Expr> value;
		explicit Unary(Token op_, std::unique_ptr<Expr> value_) noexcept :op(op_), value(std::move(value_)) {}
		Literal Accept(Visitor<Literal>& visitor) override {
			return visitor.VisitUnary(*this);
		}
		std::string Accept(Visitor<std::string>& visitor) override {
			return visitor.VisitUnary(*this);
		}
	};


	struct Variable : public Expr
	{
		std::string_view name;
		explicit Variable(std::string_view name_) : name{ name_ } {}
		Literal Accept(Visitor<Literal>& visitor) override {
			return visitor.VisitVariable(*this);
		}
		std::string Accept(Visitor<std::string>& visitor) override {
			return visitor.VisitVariable(*this);
		}
	};
}
