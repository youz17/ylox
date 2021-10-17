
#pragma once

#include <string>
#include <memory>
#include "scanner.h"
#include "literal.h"
namespace expr
{
	struct Unary;
	struct String;
	struct Number;
	struct Bool;
	struct Binary;
	struct Group;
	struct Nil;
	template<typename R>
	struct Visitor
	{
		virtual ~Visitor() = default;
		virtual R VisitUnary(Unary&) = 0;
		virtual R VisitString(String&) = 0;
		virtual R VisitNumber(Number&) = 0;
		virtual R VisitBool(Bool&) = 0;
		virtual R VisitBinary(Binary&) = 0;
		virtual R VisitGroup(Group&) = 0;
		virtual R VisitNil(Nil&) = 0;
	};

	struct Expr
	{
		virtual ~Expr() = default;
		virtual Literal Accept(Visitor<Literal>& visitor) = 0;
		virtual std::string Accept(Visitor<std::string>& visitor) = 0;
	};

	struct Unary final : public Expr
	{
		Token op;
		std::unique_ptr<Expr> value;
		Unary(Token op_, std::unique_ptr<Expr> value_) :op(op_), value(std::move(value_)) {}
		Literal Accept(Visitor<Literal>& visitor) override {
			return visitor.VisitUnary(*this);
		}
		std::string Accept(Visitor<std::string>& visitor) override {
			return visitor.VisitUnary(*this);
		}
	};

	struct String final : public Expr
	{
		std::string_view value;
		explicit String(std::string_view value_) :value(value_) {}
		Literal Accept(Visitor<Literal>& visitor) override {
			return visitor.VisitString(*this);
		}
		std::string Accept(Visitor<std::string>& visitor) override {
			return visitor.VisitString(*this);
		}
	};

	struct Number final : public Expr
	{
		double value;
		explicit Number(double value_) :value(value_) {}
		Literal Accept(Visitor<Literal>& visitor) override {
			return visitor.VisitNumber(*this);
		}
		std::string Accept(Visitor<std::string>& visitor) override {
			return visitor.VisitNumber(*this);
		}
	};

	struct Bool final : public Expr
	{
		bool value;
		explicit Bool(bool value_) :value(value_) {}
		Literal Accept(Visitor<Literal>& visitor) override {
			return visitor.VisitBool(*this);
		}
		std::string Accept(Visitor<std::string>& visitor) override {
			return visitor.VisitBool(*this);
		}
	};

	struct Binary final : public Expr
	{
		std::unique_ptr<Expr> left;
		Token op;
		std::unique_ptr<Expr> right;
		Binary(std::unique_ptr<Expr> left_, Token op_, std::unique_ptr<Expr> right_) :left(std::move(left_)), op(op_), right(std::move(right_)) {}
		Literal Accept(Visitor<Literal>& visitor) override {
			return visitor.VisitBinary(*this);
		}
		std::string Accept(Visitor<std::string>& visitor) override {
			return visitor.VisitBinary(*this);
		}
	};

	struct Group final : public Expr
	{
		std::unique_ptr<Expr> expr;
		explicit Group(std::unique_ptr<Expr> expr_) :expr(std::move(expr_)) {}
		Literal Accept(Visitor<Literal>& visitor) override {
			return visitor.VisitGroup(*this);
		}
		std::string Accept(Visitor<std::string>& visitor) override {
			return visitor.VisitGroup(*this);
		}
	};

	struct Nil final : public Expr
	{
		Nil() = default;
		Literal Accept(Visitor<Literal>& visitor) override {
			return visitor.VisitNil(*this);
		}
		std::string Accept(Visitor<std::string>& visitor) override {
			return visitor.VisitNil(*this);
		}
	};

}
