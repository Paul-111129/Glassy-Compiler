#include "Parser.h"
#include "Error.h"
#include <charconv>

namespace Glassy {

Parser::Parser(const std::vector<Token>& tokens)
    : m_Tokens(tokens), m_Index(0), m_Allocator(4 * 1024 * 1024) {} // 4 MB

Program* Parser::ParseProgram() {
    m_Index = 0;
    Program* program = m_Allocator.alloc<Program>();

    while (peek()) {
        program->statements.push_back(parseStatement());
    }

    return program;
}

Term* Parser::parseTerm() {
    auto tok = peek();
    if (!tok) {
        Error(m_Tokens.back().location, "Expected term");
    }

    if (auto t = match(LITERAL)) {
        auto termLiteral = m_Allocator.alloc<TermLiteral>(t->value.value());
        return m_Allocator.alloc<Term>(termLiteral);
    } else if (auto t = match(IDENTIFIER)) {
        auto termIdentifier = m_Allocator.alloc<TermIdentifier>(t->value.value());
        return m_Allocator.alloc<Term>(termIdentifier);
    }

    if (match(L_PAREN)) {
        auto expr = parseExpression();
        expect(R_PAREN, "Expected ')'");
        auto termParen = m_Allocator.alloc<TermParen>(expr);
        return m_Allocator.alloc<Term>(termParen);
    }

    Error(tok->location, "Unexpected token in term");
    return nullptr; // never reached
}

Expression* Parser::parseExpression() {
    auto left = m_Allocator.alloc<Expression>(parseTerm());

    while (auto tok = match(PLUS, MINUS, STAR, SLASH)) {
        auto right = m_Allocator.alloc<Expression>(parseTerm());
        left->expr = m_Allocator.alloc<ExprBinary>(tok->ToStr()[0], left, right);
    }

    return left;
}

Statement* Parser::parseStatement() {
    if (match(EXIT)) {
        auto expr = parseExpression();
        expect(SEMI, "Expected ';'");

        auto stmtExit = m_Allocator.alloc<StmtExit>(expr);
        return m_Allocator.alloc<Statement>(stmtExit);
    } else if (match(LET)) {
        std::string name = *expect(IDENTIFIER, "Expected identifer").value;

        expect(EQUAL, "Expected '='");
        auto expr = parseExpression();
        expect(SEMI, "Expected ';'");

        auto stmtLet = m_Allocator.alloc<StmtLet>(name, expr);
        return m_Allocator.alloc<Statement>(stmtLet);
    }

    std::string name = *expect(IDENTIFIER, "Expected identifier").value;

    expect(EQUAL, "Expected '='");
    auto expr = parseExpression();
    expect(SEMI, "Expected ';");

    auto stmtAssign = m_Allocator.alloc<StmtAssign>(name, expr);
    return m_Allocator.alloc<Statement>(stmtAssign);
}

} // namespace Glassy
