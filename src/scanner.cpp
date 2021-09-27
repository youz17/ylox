#include "stdafx.h"

#include "scanner.h"

#include <cctype>
#include <ctype.h>
#include <map>

#include "tools.h"

namespace scanner {
namespace ranges = std::ranges;

using T = TokenType;

constexpr std::array __tmp{'{', '}', '(', ')', '[', ']', ';', ',', '.',  '+',
                           '-', '*', '/', '=', '!', '>', '<', '"', '\\', ' '};
constexpr std::array indent_invaild_char{tool::sort(__tmp)};

const std::map<std::string_view, TokenType> keywords = {
    {"let", T::Let},     {"if", T::If},       {"else", T::Else},
    {"for", T::For},     {"and", T::And},     {"fn", T::Fn},
    {"false", T::False}, {"class", T::Class}, {"nil", T::Nil},
    {"or", T::Or},       {"print", T::Print}, {"return", T::Return},
    {"super", T::Super}, {"this", T::This},   {"true", T::True},
    {"let", T::Let},     {"while", T::While},
};

class Scanner {
  public:
    Scanner(std::string_view src)
        : m_source(src), m_line(1), m_start(0), m_current(0) {}

    std::vector<Token> tokens() {
        scan();
        std::vector<Token> tmp;
        m_tokens.swap(tmp);
        return tmp;
    }

  private:
    void scan() {
        while (m_current < m_source.length()) {
            scan_a_token();
        }

        add_token(T::Eof);
    }

    void scan_a_token() {
        m_start = m_current;
        auto c = next();

        switch (c) {
        case '(':
            add_token(T::LeftParen);
            break;
        case ')':
            add_token(T::RightParen);
            break;
        case '{':
            add_token(T::LeftBrace);
            break;
        case '}':
            add_token(T::RightBrace);
            break;
        case ',':
            add_token(T::Comma);
            break;
        case '.':
            add_token(T::Dot);
            break;
        case '+':
            add_token(T::Plus);
            break;
        case '-':
            add_token(T::Minus);
            break;
        case ';':
            add_token(T::Semicolon);
            break;
        case '*':
            add_token(T::Star);
            break;

        case '!':
            add_token(match('=') ? T::BangEqual : T::Bang);
            break;
        case '=':
            add_token(match('=') ? T::EqualEqual : T::Equal);
            break;
        case '>':
            add_token(match('=') ? T::GreaterEqual : T::Greater);
            break;
        case '<':
            add_token(match('=') ? T::Less : T::LessEqual);
            break;

        case '/':
            handle_slash();
            break;

        case '\n':
            ++m_line;
            break;

        case '\t':
        case '\r':
        case ' ':
            break;
        case '"':
            string();
            break;

        default:
            if (std::isdigit(c)) {
                number();
            } else if (std::isalpha(c) || c == '_') {
                identifier();
            } else {
                // todo: error
            }
        }
    }

    void handle_slash() {
        if (peek() == '/') {
            while (is_not_end() && peek() != '\n')
                next();
        } else {
            add_token(T::Slash);
        }
    }

    void string() {
        while (is_not_end() && peek() != '"') {
            m_current++;
        }
        add_token_with_value(
            T::Str, m_source.substr(m_start + 1, m_current - m_start - 1));
    }

    void number() {
        // 这里允许数字中多个小数点，错误在 ast 阶段处理
        char c = peek();
        while (is_not_end() && (isdigit(c) || c == '.')) {
            m_current++;
            c = peek();
        }
        add_token_with_value(T::Number,
                             m_source.substr(m_start, m_current - m_start));
    }

    void identifier() {
        while (is_not_end() &&
               !ranges::binary_search(indent_invaild_char, peek())) {
            m_current++;
        }
        auto value = m_source.substr(m_start, m_current - m_start);

        auto iter = keywords.find(value);
        if (iter != keywords.end()) {
            add_token(iter->second);
        } else {
            add_token_with_value(T::Identifier, value);
        }
    }

    void add_token(TokenType t) { m_tokens.emplace_back(t, m_line); }

    void add_token_with_value(TokenType t, std::string_view v) {
        m_tokens.emplace_back(t, v, m_line);
    }

    char next() {
        if (m_current >= m_source.length())
            return 0;
        return m_source[m_current++];
    }

    bool match(char expect) {
        if (peek() == expect) {
            m_current++;
            return true;
        }
        return false;
    }

    bool is_not_end() { return m_current < m_source.length(); }

    char peek() {
        if (m_current >= m_source.length())
            return 0;
        return m_source[m_current];
    }

    char peek_next() {
        if (m_current + 1 >= m_source.length())
            return 0;
        return m_source[m_current + 1];
    }

  private:
    std::string_view m_source;
    std::vector<Token> m_tokens;
    int m_line;
    size_t m_start = 0;
    size_t m_current = 0;
};

} // namespace scanner

std::vector<Token> scan(std::string_view source) {
    scanner::Scanner s(source);
    return std::move(s.tokens());
}

std::string to_string(TokenType t) {
    using T = TokenType;
    switch (t) {
    case T::LeftParen:
        return "'('";
    case T::RightParen:
        return "')'";
    case T::LeftBrace:
        return "'{'";
    case T::RightBrace:
        return "'}'";
    case T::Comma:
        return "':'";
    case T::Dot:
        return "'.'";
    case T::Minus:
        return "-";
    case T::Plus:
        return "+";
    case T::Semicolon:
        return "';'";
    case T::Slash:
        return "/";
    case T::Star:
        return "*";
    case T::Bang:
        return "!";
    case T::BangEqual:
        return "!=";
    case T::Equal:
        return "=";
    case T::EqualEqual:
        return "==";
    case T::Greater:
        return ">";
    case T::GreaterEqual:
        return ">=";
    case T::Less:
        return "<";
    case T::LessEqual:
        return "<=";
    case T::Identifier:
        return "ident";
    case T::Str:
        return "str";
    case T::Number:
        return "num";
    case T::If:
        return "if";
    case T::Else:
        return "else";
    case T::For:
        return "for";
    case T::And:
        return "and";
    case T::Fn:
        return "fn";
    case T::False:
        return "false";
    case T::Class:
        return "class";
    case T::Nil:
        return "nil";
    case T::Or:
        return "or";
    case T::Print:
        return "print";
    case T::Return:
        return "return";
    case T::Super:
        return "super";
    case T::This:
        return "super";
    case T::True:
        return "true";
    case T::Let:
        return "let";
    case T::While:
        return "while";
    case T::Eof:
        return "eof";
    default:
        assert(false);
        return "";
    }
}