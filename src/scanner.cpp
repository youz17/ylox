#include "stdafx.h"

#include "scanner.h"

#include <cctype>
#include <map>

#include "error.h"
#include "tools.h"

namespace scanner
{
	namespace ranges = std::ranges;

	using TT = TokenType;

	constexpr std::array TMP{ '{', '}', '(', ')', '[', ']', ';', ',', '.',  '+',
							   '-', '*', '/', '=', '!', '>', '<', '"', '\\', ' ' };
	constexpr std::array INDENT_INVALID_CHAR{ tool::Sort(TMP) };

	const std::map<std::string_view, TokenType> KEYWORDS = {
		{"let", TT::Let},     {"if", TT::If},       {"else", TT::Else},
		{"for", TT::For},     {"and", TT::And},     {"fn", TT::Fn},
		{"false", TT::False}, {"class", TT::Class}, {"nil", TT::Nil},
		{"or", TT::Or},       {"print", TT::Print}, {"return", TT::Return},
		{"super", TT::Super}, {"this", TT::This},   {"true", TT::True},
		{"let", TT::Let},     {"while", TT::While},
	};

	class Scanner
	{
	public:
		explicit Scanner(std::string_view src)
			: m_source(src)
		{
		}

		std::vector<Token> Tokens()
		{
			_Scan();
			std::vector<Token> tmp;
			m_tokens.swap(tmp);
			return tmp;
		}

	private:
		void _Scan()
		{
			while (m_current < m_source.length())
			{
				_ScanAToken();
			}

			_AddToken(TT::Eof);
		}

		void _ScanAToken()
		{
			m_start = m_current;

			switch (auto c = _Next())
			{
			case '(':
				_AddToken(TT::LeftParen);
				break;
			case ')':
				_AddToken(TT::RightParen);
				break;
			case '{':
				_AddToken(TT::LeftBrace);
				break;
			case '}':
				_AddToken(TT::RightBrace);
				break;
			case ',':
				_AddToken(TT::Comma);
				break;
			case '.':
				_AddToken(TT::Dot);
				break;
			case '+':
				_AddToken(TT::Plus);
				break;
			case '-':
				_AddToken(TT::Minus);
				break;
			case ';':
				_AddToken(TT::Semicolon);
				break;
			case '*':
				_AddToken(TT::Star);
				break;

			case '!':
				_AddToken(_Match('=') ? TT::BangEqual : TT::Bang);
				break;
			case '=':
				_AddToken(_Match('=') ? TT::EqualEqual : TT::Equal);
				break;
			case '>':
				_AddToken(_Match('=') ? TT::GreaterEqual : TT::Greater);
				break;
			case '<':
				_AddToken(_Match('=') ? TT::Less : TT::LessEqual);
				break;

			case '/':
				_HandleSlash();
				break;

			case '\n':
				++m_line;
				break;

			case '\t':
			case '\r':
			case ' ':
				break;
			case '"':
				_String();
				break;

			default:
				if (std::isdigit(c))
					_Number();
				else if (std::isalpha(c) || c == '_')
					_Identifier();
				else
					throw ScanError(m_line, c);
			}
		}

		void _HandleSlash()
		{
			if (_Peek() == '/')
			{
				while (_IsNotEnd() && _Peek() != '\n')
					_Next();
			}
			else
			{
				_AddToken(TT::Slash);
			}
		}

		void _String()
		{
			while (_IsNotEnd() && _Peek() != '"')
			{
				m_current++;
			}
			m_current++;
			_AddTokenWithValue(
				TT::Str, m_source.substr(m_start + 1, m_current - m_start - 2));
		}

		void _Number()
		{
			// 这里允许数字中多个小数点，错误在 ast 阶段处理
			char c = _Peek();
			while (_IsNotEnd() && (isdigit(c) || c == '.'))
			{
				m_current++;
				c = _Peek();
			}
			_AddTokenWithValue(TT::Number,
				m_source.substr(m_start, m_current - m_start));
		}

		void _Identifier()
		{
			while (_IsNotEnd() &&
				!ranges::binary_search(INDENT_INVALID_CHAR, _Peek()))
			{
				m_current++;
			}
			const auto value = m_source.substr(m_start, m_current - m_start);

			const auto iter = KEYWORDS.find(value);
			if (iter != KEYWORDS.end())
			{
				_AddToken(iter->second);
			}
			else
			{
				_AddTokenWithValue(TT::Identifier, value);
			}
		}

		void _AddToken(TokenType t) { m_tokens.emplace_back(t, m_line); }

		void _AddTokenWithValue(TokenType t, std::string_view v)
		{
			m_tokens.emplace_back(t, v, m_line);
		}

		char _Next()
		{
			if (!_IsNotEnd())
				return 0;
			return m_source[m_current++];
		}

		bool _Match(char expect)
		{
			if (_Peek() == expect)
			{
				m_current++;
				return true;
			}
			return false;
		}

		[[nodiscard]] bool _IsNotEnd() const { return m_current < m_source.length(); }

		[[nodiscard]] char _Peek() const
		{
			if (m_current >= m_source.length())
				return 0;
			return m_source[m_current];
		}

		[[nodiscard]] char _PeekNext() const
		{
			if (m_current + 1 >= m_source.length())
				return 0;
			return m_source[m_current + 1];
		}

	private:
		std::string_view m_source;
		std::vector<Token> m_tokens;
		int m_line = 1;
		size_t m_start = 0;
		size_t m_current = 0;
	};

} // namespace scanner

std::vector<Token> Scan(std::string_view source)
{
	scanner::Scanner s(source);
	return s.Tokens();
}

std::string ToString(TokenType t)
{
	using TT = TokenType;
	switch (t)
	{
	case TT::LeftParen:
		return "'('";
	case TT::RightParen:
		return "')'";
	case TT::LeftBrace:
		return "'{'";
	case TT::RightBrace:
		return "'}'";
	case TT::Comma:
		return "':'";
	case TT::Dot:
		return "'.'";
	case TT::Minus:
		return "-";
	case TT::Plus:
		return "+";
	case TT::Semicolon:
		return "';'";
	case TT::Slash:
		return "/";
	case TT::Star:
		return "*";
	case TT::Bang:
		return "!";
	case TT::BangEqual:
		return "!=";
	case TT::Equal:
		return "=";
	case TT::EqualEqual:
		return "==";
	case TT::Greater:
		return ">";
	case TT::GreaterEqual:
		return ">=";
	case TT::Less:
		return "<";
	case TT::LessEqual:
		return "<=";
	case TT::Identifier:
		return "ident";
	case TT::Str:
		return "str";
	case TT::Number:
		return "num";
	case TT::If:
		return "if";
	case TT::Else:
		return "else";
	case TT::For:
		return "for";
	case TT::And:
		return "and";
	case TT::Fn:
		return "fn";
	case TT::False:
		return "false";
	case TT::Class:
		return "class";
	case TT::Nil:
		return "nil";
	case TT::Or:
		return "or";
	case TT::Print:
		return "print";
	case TT::Return:
		return "return";
	case TT::Super:
		return "super";
	case TT::This:
		return "this";
	case TT::True:
		return "true";
	case TT::Let:
		return "let";
	case TT::While:
		return "while";
	case TT::Eof:
		return "eof";
	default:
		assert(false);
		return "";
	}
}