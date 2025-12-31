#include "pch.h"
#include "Tokenizer.h"

namespace Glassy {

Tokenizer::Tokenizer(const std::string &src) : m_Src(src) {}

std::vector<Token> Tokenizer::Tokenize() const {
    const size_t size = m_Src.size();

    std::vector<Token> tokens;
    std::string buf;

    uint16_t line = 1;

    for (size_t i = 0; i < size; ++i) {
        const char c = m_Src[i];

        if (std::isspace(static_cast<unsigned char>(c))) {
            if (c == '\n') {
                line++;
            }
            continue;
        } else if (std::isalpha(static_cast<unsigned char>(c))) {
            while (i < size && std::isalnum(m_Src[i])) {
                buf.push_back(m_Src[i++]);
            }
            --i;

            auto it = keywords.find(buf);
            if (it != keywords.end()) {
                tokens.emplace_back(it->second, line); // keyword
            } else {
                tokens.emplace_back(buf, line); // identifier
            }

            buf.clear();
            continue;
        } else if (c == '.' || std::isdigit(static_cast<unsigned char>(m_Src[i]))) {
            bool hasDot = false;

            while (i < size && (m_Src[i] == '.' || std::isdigit(static_cast<unsigned char>(m_Src[i])))) {
                if (m_Src[i] == '.') {
                    if (hasDot) {
                        Error(line, "More than 1 dot in literal");
                    }
                    hasDot = true;
                }
                buf.push_back(m_Src[i++]);
            }
            --i;

            try {
                tokens.emplace_back(std::stod(buf), line); // literal
            } catch (...) {
                Error(line, "Failed to parse token '", buf, "'");
            }

            buf.clear();
            continue;
        }

        switch (c) {
            // operators
            case '+':
                tokens.emplace_back(Operator::PLUS, line);
                break;
            case '-':
                tokens.emplace_back(Operator::MINUS, line);
                break;
            case '*':
                tokens.emplace_back(Operator::STAR, line);
                break;
            case '/':
                tokens.emplace_back(Operator::SLASH, line);
                break;
            case '%':
                tokens.emplace_back(Operator::PERCENT, line);
                break;
            case '^':
                tokens.emplace_back(Operator::CARET, line);
                break;
            case '=':
                tokens.emplace_back(Operator::EQUAL, line);
                break;

            // separators
            case '(':
                tokens.emplace_back(Separator::L_PAREN, line);
                break;
            case ')':
                tokens.emplace_back(Separator::R_PAREN, line);
                break;
            case ';':
                tokens.emplace_back(Separator::SEMI, line);
                break;
            default:
                Error(line, "unrecognized token '", c, "'");
        }
    }

    return tokens;
}

} // namespace Glassy
