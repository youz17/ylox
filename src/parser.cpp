#include "stdafx.h"

#include "parser.h"

namespace
{
	enum class ExprType
	{
		Equality,
		Comparison,
		Term,
		Factor,
		Unary,
		Primary,
		Other,
	};

	ExprType GetBinTokenExprType(const Token& t)
	{
		switch (t.type)
		{
		case TokenType::BangEqual:
		case TokenType::EqualEqual:
			return ExprType::Equality;
		case TokenType::GreaterEqual:
		case TokenType::LessEqual:
		case TokenType::Greater:
		case TokenType::Less:
			return ExprType::Comparison;
		case TokenType::Plus:
		case TokenType::Minus:
			return ExprType::Term;
		case TokenType::Star:
		case TokenType::Slash:
			return ExprType::Factor;
		default:
			return ExprType::Other;
		}
	}

	bool IsUnary(const Token& t)
	{
		return t.type == TokenType::Plus || t.type == TokenType::Bang;
	}

	class Parser
	{
	public:
		explicit Parser(const std::vector<Token>& tokens) noexcept : m_tokens(tokens) {}

		std::unique_ptr<Expr> Expression()
		{
			return Equality();
		}

	private:
		std::unique_ptr<Expr> Equality()
		{
			auto expr = Comparison();

			while (GetBinTokenExprType(Peek()) == ExprType::Equality)
			{
				const TokenType op = Next().type;
				auto right = Comparison();
				expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
			}

			return expr;
		}

		std::unique_ptr<Expr> Comparison()
		{
			auto expr = Term();

			while (GetBinTokenExprType(Peek()) == ExprType::Comparison)
			{
				const TokenType op = Next().type;
				auto right = Term();
				expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
			}

			return expr;
		}

		std::unique_ptr<Expr> Term()
		{
			auto expr = Factor();

			while (GetBinTokenExprType(Peek()) == ExprType::Term)
			{
				const TokenType op = Next().type;
				auto right = Factor();
				expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
			}

			return expr;
		}

		std::unique_ptr<Expr> Factor()
		{
			auto expr = Unary();

			while (GetBinTokenExprType(Peek()) == ExprType::Factor)
			{
				const TokenType op = Next().type;
				auto right = Unary();
				expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
			}

			return expr;
		}

		std::unique_ptr<Expr> Unary()
		{
			if (IsUnary(Peek()))
			{
				const TokenType op = Next().type;
				return std::make_unique<struct Unary>(op, Unary());
			}
			return Primary();
		}

		std::unique_ptr<Expr> Primary()
		{
			const auto token = Next();
			switch (token.type)
			{
			case TokenType::Str:
				return make_unique<String>(token.value);
			case TokenType::Number:
			{
				double res = 0;
				std::from_chars(token.value.data(), token.value.data() + token.value.size(), res);
				return make_unique<Number>(res);
			}
			case TokenType::True:
				return make_unique<Bool>(true);
			case TokenType::False:
				return make_unique<Bool>(false);
			case TokenType::Nil:
				return make_unique<Nil>();
			case TokenType::LeftParen:
			{
				auto expr = Expression();
				const auto next = Next();
				assert(next.type == TokenType::RightParen);
				return make_unique<Group>(std::move(expr));
			}
			default:
				assert(false);
			}
		}

		const Token& Next()
		{
			return m_tokens[m_idx++];
		}

		[[nodiscard]] const Token& Peek() const
		{
			return m_tokens[m_idx];
		}

		[[nodiscard]] const Token& PeekNext() const
		{

			return m_tokens[m_idx + 1];
		}

		[[nodiscard]] bool IsEnd() const
		{
			return m_tokens[m_idx].type == TokenType::Eof;
		}

	private:
		const std::vector<Token>& m_tokens;
		size_t m_idx = 0;
	};
}

unique_ptr<Expr> Parse(const vector<Token>& tokens)
{
	Parser parser(tokens);
	return parser.Expression();
}
