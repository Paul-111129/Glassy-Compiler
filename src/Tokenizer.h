#pragma once

#include "pch.h"

namespace Glassy {

template <typename... Args>
inline void Error(uint16_t line, Args &&...args) {
    (std::cerr << "At line " << line << ": " << ... << std::forward<Args>(args)) << "\n";
    std::cin.get();
    std::exit(1);
}

// clang-format off

    enum TokenType { 
        IDENTIFIER,
        KEYWORD,
        SEPARATOR,
        OPERATOR,
        LITERAL
    };

    enum class Keyword {
        EXIT,
        LET,
        KEYWORD_NB
    };
    inline const std::unordered_map<std::string_view, Keyword> keywords = {
        { "exit", Keyword::EXIT },
        {  "let", Keyword::LET  }
    };

    enum class Separator {
        L_PAREN,
        R_PAREN,
        L_BRACKET,
        R_BRACKET,
        L_BRACE,
        R_BRACE,
        SEMI,
        SEPARATOR_NB
    };
    inline const std::unordered_map<char, Separator> separators = {
        {'(', Separator::L_PAREN},
        {')', Separator::R_PAREN},
        {'[', Separator::L_BRACKET},
        {']', Separator::R_BRACKET},
        {'{', Separator::L_BRACE},
        {'}', Separator::R_BRACE},
        {';', Separator::SEMI}
    };

    enum class Operator {
        PLUS,
        MINUS,
        STAR,
        SLASH,
        PERCENT,
        CARET,
        EQUAL,
        OPERATOR_NB
    };
    inline const char OperatorToChar[int(Operator::OPERATOR_NB)] = {
        '+', 
        '-',
        '*',
        '/',
        '%',
        '^',
        '='
    };

// clang-format on

using SubType = std::variant<Keyword, Separator, Operator>;

struct Token {
    Token(const std::string &id, uint16_t ln) : type(IDENTIFIER), identifer(id), line(ln) {}

    Token(Keyword kw, uint16_t ln) : type(KEYWORD), subType(kw), line(ln) {}

    Token(double val, uint16_t ln) : type(LITERAL), value(val), line(ln) {}

    Token(Operator op, uint16_t ln) : type(OPERATOR), subType(op), line(ln) {}

    Token(Separator sep, uint16_t ln) : type(SEPARATOR), subType(sep), line(ln) {}

    TokenType type;
    std::optional<SubType> subType = std::nullopt;

    std::optional<std::string> identifer = std::nullopt; // valid if type == IDENTIFIER
    std::optional<double> value = std::nullopt; // valid if type == LITERAL

    uint16_t line;
};

class Tokenizer {
  public:
    explicit Tokenizer(const std::string &src);
    std::vector<Token> Tokenize() const;

  private:
    const std::string m_Src;
};

} // namespace Glassy