#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace Glassy {

enum TokenType {
    IDENTIFIER,
    LITERAL,

    EXIT,
    LET,

    L_PAREN,
    R_PAREN,
    L_BRACKET,
    R_BRACKET,
    L_BRACE,
    R_BRACE,
    SEMI,

    PLUS,
    MINUS,
    STAR,
    SLASH,
    PERCENT,
    CARET,
    EQUAL,

    TOKEN_TYPE_NB
};

struct SourceLocation {
    uint16_t line = 1;
    uint16_t column = 1;
};

struct Token {
    Token(TokenType type, SourceLocation loc) : type(type), location(loc) {}
    Token(TokenType type, SourceLocation loc, std::string_view v) : type(type), location(loc), value(v) {}

    const char* ToStr() const { return TokenToStr[type]; }

    static constexpr const char* TokenToStr[TOKEN_TYPE_NB] = { "identifier", "literal", "exit", "let", "(",
        ")", "[", "]", "{", "}", ";", "+", "-", "*", "/", "%", "^", "=" };

    TokenType type;
    SourceLocation location;
    std::optional<std::string> value = std::nullopt; // literal or identifier
};

class Tokenizer {
  public:
    explicit Tokenizer(std::string_view src);
    std::vector<Token> Tokenize() const;

  private:
    std::string_view m_Src;
};

} // namespace Glassy
