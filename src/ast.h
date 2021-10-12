#pragma once

#include <string>
#include <memory>
#include <variant>
#include "scanner.h"
#include "literal.h"

namespace ast
{
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
		virtual R VisitUnaryExpr(Unary&) = 0;
		virtual R VisitStringExpr(String&) = 0;
		virtual R VisitNumberExpr(Number&) = 0;
		virtual R VisitBoolExpr(Bool&) = 0;
		virtual R VisitBinaryExpr(Binary&) = 0;
		virtual R VisitGroupExpr(Group&) = 0;
		virtual R VisitNilExpr(Nil&) = 0;
	};

	struct Expr
	{
		virtual ~Expr() = default;
		virtual Literal Accept(AstVisitor<Literal>& visitor) = 0;
		virtual std::string Accept(AstVisitor<std::string>& visitor) = 0;
	};

	struct Unary final : public Expr
	{
		TokenType op;
		std::unique_ptr<Expr> value;
		Unary(TokenType op_, std::unique_ptr<Expr> value_) :op(op_), value(std::move(value_)) {}
		Literal Accept(AstVisitor<Literal>& visitor) override {
			return visitor.VisitUnaryExpr(*this);
		}
		std::string Accept(AstVisitor<std::string>& visitor) override {
			return visitor.VisitUnaryExpr(*this);
		}
	};

	struct String final : public Expr
	{
		std::string_view value;
		explicit String(std::string_view value_) :value(value_) {}
		Literal Accept(AstVisitor<Literal>& visitor) override {
			return visitor.VisitStringExpr(*this);
		}
		std::string Accept(AstVisitor<std::string>& visitor) override {
			return visitor.VisitStringExpr(*this);
		}
	};

	struct Number final : public Expr
	{
		double value;
		explicit Number(double value_) :value(value_) {}
		Literal Accept(AstVisitor<Literal>& visitor) override {
			return visitor.VisitNumberExpr(*this);
		}
		std::string Accept(AstVisitor<std::string>& visitor) override {
			return visitor.VisitNumberExpr(*this);
		}
	};

	struct Bool final : public Expr
	{
		bool value;
		explicit Bool(bool value_) :value(value_) {}
		Literal Accept(AstVisitor<Literal>& visitor) override {
			return visitor.VisitBoolExpr(*this);
		}
		std::string Accept(AstVisitor<std::string>& visitor) override {
			return visitor.VisitBoolExpr(*this);
		}
	};

	struct Binary final : public Expr
	{
		std::unique_ptr<Expr> left;
		TokenType op;
		std::unique_ptr<Expr> right;
		Binary(std::unique_ptr<Expr> left_, TokenType op_, std::unique_ptr<Expr> right_) :left(std::move(left_)), op(op_), right(std::move(right_)) {}
		Literal Accept(AstVisitor<Literal>& visitor) override {
			return visitor.VisitBinaryExpr(*this);
		}
		std::string Accept(AstVisitor<std::string>& visitor) override {
			return visitor.VisitBinaryExpr(*this);
		}
	};

	struct Group final : public Expr
	{
		std::unique_ptr<Expr> expr;
		explicit Group(std::unique_ptr<Expr> expr_) :expr(std::move(expr_)) {}
		Literal Accept(AstVisitor<Literal>& visitor) override {
			return visitor.VisitGroupExpr(*this);
		}
		std::string Accept(AstVisitor<std::string>& visitor) override {
			return visitor.VisitGroupExpr(*this);
		}
	};

	struct Nil final : public Expr
	{
		Nil() = default;
		Literal Accept(AstVisitor<Literal>& visitor) override {
			return visitor.VisitNilExpr(*this);
		}
		std::string Accept(AstVisitor<std::string>& visitor) override {
			return visitor.VisitNilExpr(*this);
		}
	};
}
