#pragma once

#include <format>
#include <string_view>
#include <vector>

enum class TokenType {
    // Single-character tokens.
    LeftParen,
    RightParen,
    LeftBrace,
    RightBrace,
    Comma,
    Dot,
    Minus,
    Plus,
    Semicolon,
    Slash,
    Star,

    // One or two character tokens.
    Bang,
    BangEqual,
    Equal,
    EqualEqual,
    Greater,
    GreaterEqual,
    Less,
    LessEqual,

    // Literals.
    Identifier,
    Str,
    Number,

    // Keywords.
    If,
    Else,
    For,
    And,
    Fn,
    False,
    Class,
    Nil,
    Or,
    Print,
    Return,
    Super,
    This,
    True,
    Let,
    While,

    Eof,
};

struct Token {
    TokenType type;
    std::string_view value;
    int line;

    Token(TokenType t, int l) : type(t), line(l) {}

    Token(TokenType t, std::string_view v, int l)
        : type(t), value(v), line(l) {}
};

// 为了简单起见， 目前直接使用 std::string_view , 只支持 ascii
std::vector<Token> Scan(std::string_view source);

////////////////// help function /////////////////////

std::string to_string(TokenType t);

template <> struct std::formatter<TokenType> : std::formatter<std::string> {
    auto format(TokenType t, std::format_context &fc) {
        return formatter<string>::format(to_string(t), fc);
    }
};

template <> struct std::formatter<Token> : std::formatter<std::string> {
    auto format(Token t, std::format_context &fc) {
        if (t.value.size() == 0) {
            return formatter<string>::format(
                std::format("({}: {})", t.type, t.line, t.value), fc);
        }
        return formatter<string>::format(
            std::format("({}: {}, {})", t.type, t.line, t.value), fc);
    }
};
