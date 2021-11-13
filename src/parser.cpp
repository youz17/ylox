#include "stdafx.h"

#include "parser.h"

#include <functional>

#include "error.h"

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

		StmtVec Parse()
		{
			StmtVec stmts;

			while (!IsEnd())
			{
				stmts.push_back(Declaration());
			}

			return stmts;
		}

	private:
		std::unique_ptr<stmt::Stmt> Declaration()
		{
			try
			{
				auto next = Peek();
				switch (next.type)
				{
				case TokenType::Let:
					Next();
					return LetDeclaration();
				default:
					return Statement();
				}
			}
			catch (ParseError&)
			{
				Synchronize();
			}
		}

		std::unique_ptr<stmt::Stmt> LetDeclaration()
		{
			auto id = Next();
			if (id.type != TokenType::Identifier)
				throw ParseError(id, "follow by let is not a Identifier");
			Consume(TokenType::Equal, "let statement expect '='");
			
			auto let = make_unique<stmt::Let>(id.value, Expression());
			Consume(TokenType::Semicolon, "expect ';' after print");
			return let;
		}

		std::unique_ptr<stmt::Stmt> Statement()
		{
			auto next = Next();
			switch (next.type)
			{
			case TokenType::Print:
				return PrintStmt();
			default:
				return ExpressionStmt();
			}
		}

		std::unique_ptr<stmt::Stmt> PrintStmt()
		{
			auto expr = make_unique<stmt::Print>(Expression());

			Consume(TokenType::Semicolon, "expect ';' after print");

			return expr;
		}

		unique_ptr<stmt::Stmt> ExpressionStmt() {
			unique_ptr<expr::Expr> expr = Expression();
			Consume(TokenType::Semicolon, "Expect ';' after expression.");
			return make_unique<stmt::Expression>(std::move(expr));
		}

		std::unique_ptr<expr::Expr> Expression()
		{
			return Logical();
		}

		using BinOperatorImplFunType = unique_ptr<expr::Expr>(Parser::*)();
		std::unique_ptr<expr::Expr> BinOperatorImpl(ExprType exprType, BinOperatorImplFunType subExprFun)
		{
			auto expr = (this->*subExprFun)();

			while (GetBinTokenExprType(Peek()) == exprType)
			{
				const Token token = Next();
				auto right = (this->*subExprFun)();
				expr = std::make_unique<expr::Binary>(std::move(expr), token, std::move(right));
			}

			return expr;
		}

		// 好像可以进一步抽象，只定义优先级就好了
		std::unique_ptr<expr::Expr> Logical()
		{
			return BinOperatorImpl(ExprType::Logical, &Parser::Equality);
		}

		std::unique_ptr<expr::Expr> Equality()
		{
			return BinOperatorImpl(ExprType::Equality, &Parser::Comparison);
		}

		std::unique_ptr<expr::Expr> Comparison()
		{
			return BinOperatorImpl(ExprType::Comparison, &Parser::Term);
		}

		std::unique_ptr<expr::Expr> Term()
		{
			return BinOperatorImpl(ExprType::Term, &Parser::Factor);
		}

		std::unique_ptr<expr::Expr> Factor()
		{
			return BinOperatorImpl(ExprType::Factor, &Parser::Unary);
		}

		std::unique_ptr<expr::Expr> Unary()
		{
			if (IsUnary(Peek()))
			{
				const Token op = Next();
				return std::make_unique<expr::Unary>(op, Unary());
			}
			return Primary();
		}

		std::unique_ptr<expr::Expr> Primary()
		{
			const auto& token = Next();
			switch (token.type)
			{
			case TokenType::Str:
				return make_unique<expr::String>(token.value);
			case TokenType::Number:
			{
				double res = 0;
				std::from_chars(token.value.data(), token.value.data() + token.value.size(), res); // todo: error handle
				return make_unique<expr::Number>(res);
			}
			case TokenType::True:
				return make_unique<expr::Bool>(true);
			case TokenType::False:
				return make_unique<expr::Bool>(false);
			case TokenType::Nil:
				return make_unique<expr::Nil>();
			case TokenType::LeftParen:
			{
				auto expr = Expression();
				const auto& next = Next();
				if (next.type != TokenType::RightParen)
					throw ParseError(next, "缺少匹配的 右括号");
				return make_unique<expr::Group>(std::move(expr));
			}
			case TokenType::Identifier:
				return make_unique<expr::Variable>(token.value);
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

		void Consume(TokenType expect, std::string msg)
		{
			auto next = Next();
			if (next.type == expect)
			{
				return;
			}
			throw ParseError(next, msg);
		}

	private:
		const std::vector<Token>& m_tokens;
		size_t m_idx = 0;
	};
}

vector<unique_ptr<stmt::Stmt>> Parse(const vector<Token>& tokens)
{
	Parser parser(tokens);
	return parser.Parse();
}
