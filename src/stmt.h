#pragma once

#include <string>
#include <memory>
#include "scanner.h"
#include "literal.h"

namespace stmt
{
	struct Expression;
	struct Print;
	struct Let;
	// struct If;
	// struct IfElse;

	template<typename R>
	struct Visitor
	{
		virtual ~Visitor() = default;

		virtual R VisitExpression(Expression&) = 0;
		virtual R VisitPrint(Print&) = 0;
		virtual R VisitLet(Let&) = 0;
	};

	struct Stmt
	{
		virtual ~Stmt() = default;

		virtual void Accept(Visitor<void>&) = 0;
	};

	struct Expression : public Stmt
	{
		std::unique_ptr<expr::Expr> expr;
		explicit Expression(std::unique_ptr<expr::Expr> expr_) noexcept :expr(std::move(expr_)) {}
		void Accept(Visitor<void>& visitor) override {
			return visitor.VisitExpression(*this);
		}

	};

	struct Print : public Stmt
	{
		std::unique_ptr<expr::Expr> expr;
		explicit Print(std::unique_ptr<expr::Expr> expr_) noexcept :expr(std::move(expr_)) {}
		void Accept(Visitor<void>& visitor) override {
			return visitor.VisitPrint(*this);
		}
	};

	struct Let : public Stmt
	{
		std::string name;
		std::unique_ptr<expr::Expr> expr;
		explicit Let(std::string_view name_, std::unique_ptr<expr::Expr> expr_) noexcept :name(name_), expr(std::move(expr_)) {}
		void Accept(Visitor<void>& visitor) override {
			return visitor.VisitLet(*this);
		}
	};
}
