#include "stdafx.h"

#include "parser.h"

#include <functional>

#include "error.h"

using namespace ast;
// todo: error handle, & and | , &&, ||
namespace
{
	enum class ExprType
	{
		Equality,
		Comparison,
		Term,
		Factor,
		Logical,
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
		case TokenType::And:
		case TokenType::Or:
			return ExprType::Logical;
		default:
			return ExprType::Other;
		}
	}

	bool IsUnary(const Token& t)
	{
		return t.type == TokenType::Plus || t.type == TokenType::Bang;
	}

	bool IsStartToken(const Token& t)
	{
		switch (t.type)
		{
		case TokenType::If:
		case TokenType::For:
		case TokenType::While:
		case TokenType::Let:
		case TokenType::Class:
		case TokenType::Fn:
		case TokenType::Print:
		case TokenType::Return:
			return true;
		default:
			return false;
		}
	}

	class Parser
	{
	public:
		explicit Parser(const std::vector<Token>& tokens) noexcept : m_tokens(tokens) {}

		std::unique_ptr<Expr> Expression()
		{
			return Logical();
		}

	private:
		using BinOperatorImplFunType = unique_ptr<Expr>(Parser::*)();
		std::unique_ptr<Expr> BinOperatorImpl(ExprType exprType, BinOperatorImplFunType subExprFun)
		{
			auto expr = (this->*subExprFun)();

			while (GetBinTokenExprType(Peek()) == exprType)
			{
				const TokenType op = Next().type;
				auto right = (this->*subExprFun)();
				expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
			}

			return expr;
		}

		// 好像可以进一步抽象，只定义优先级就好了
		std::unique_ptr<Expr> Logical()
		{
			return BinOperatorImpl(ExprType::Logical, &Parser::Equality);
		}

		std::unique_ptr<Expr> Equality()
		{
			return BinOperatorImpl(ExprType::Equality, &Parser::Comparison);
		}

		std::unique_ptr<Expr> Comparison()
		{
			return BinOperatorImpl(ExprType::Comparison, &Parser::Term);
		}

		std::unique_ptr<Expr> Term()
		{
			return BinOperatorImpl(ExprType::Term, &Parser::Factor);
		}

		std::unique_ptr<Expr> Factor()
		{
			return BinOperatorImpl(ExprType::Factor, &Parser::Unary);
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
			const auto& token = Next();
			switch (token.type)
			{
			case TokenType::Str:
				return make_unique<String>(token.value);
			case TokenType::Number:
			{
				double res = 0;
				std::from_chars(token.value.data(), token.value.data() + token.value.size(), res); // todo: error handle
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
				const auto& next = Next();
				assert(next.type == TokenType::RightParen);
				return make_unique<Group>(std::move(expr));
			}
			case TokenType::Identifier:
				// todo
			default:
				throw ParseError(token);
			}
		}

		// 当前语句出错的时候跳过这个语句
		void Synchronize()
		{
			while (!IsEnd())
			{
				const auto& next = PeekNext();
				if (next.type == TokenType::Eof)
					return;
				if (IsStartToken(next))
					return;
				m_idx++;
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
