#include "Tokenizer.h"
#include "Error.h"

#include <cctype>
#include <charconv>
#include <format>
#include <iostream>

namespace Glassy {

Tokenizer::Tokenizer(std::string_view src) : m_Src(src) {}

std::vector<Token> Tokenizer::Tokenize() const {
    const size_t size = m_Src.size();

    std::vector<Token> tokens;
    SourceLocation loc;

    for (size_t i = 0; i < size;) {
        const char c = m_Src[i];

        if (std::isspace(static_cast<unsigned char>(c))) {
            if (c == '\n') {
                ++loc.line;
                loc.column = 0;
            }

            ++i;
            ++loc.column;
            continue;
        }

        SourceLocation startLoc = loc;

        if (std::isalpha(static_cast<unsigned char>(c))) {
            const size_t start = i;

            while (i < size && std::isalnum(static_cast<unsigned char>(m_Src[i]))) {
                ++i;
                ++loc.column;
            }

            std::string_view lex(m_Src.data() + start, i - start);

            if (lex == "exit") {
                tokens.emplace_back(EXIT, startLoc);
            } else if (lex == "let") {
                tokens.emplace_back(LET, startLoc);
            } else {
                tokens.emplace_back(IDENTIFIER, startLoc, lex);
            }
            continue;
        } else if (std::isdigit(static_cast<unsigned char>(c))) {
            const size_t start = i;

            while (i < size && (std::isdigit(static_cast<unsigned char>(m_Src[i])))) {
                ++i;
                ++loc.column;
            }

            std::string_view lex(m_Src.data() + start, i - start);
            tokens.emplace_back(LITERAL, startLoc, lex);
            continue;
        }

        switch (c) {
            // operators
            case '+': tokens.emplace_back(PLUS, startLoc); break;
            case '-': tokens.emplace_back(MINUS, startLoc); break;
            case '*': tokens.emplace_back(STAR, startLoc); break;
            case '/': tokens.emplace_back(SLASH, startLoc); break;
            case '%': tokens.emplace_back(PERCENT, startLoc); break;
            case '^': tokens.emplace_back(CARET, startLoc); break;
            case '=': tokens.emplace_back(EQUAL, startLoc); break;

            // separators
            case '(': tokens.emplace_back(L_PAREN, startLoc); break;
            case ')': tokens.emplace_back(R_PAREN, startLoc); break;
            case ';': tokens.emplace_back(SEMI, startLoc); break;

            default: Error(startLoc, std::format("Unrecognized token '{}'", c));
        }

        ++i;
        ++loc.column;
    }

    return tokens;
}

} // namespace Glassy
