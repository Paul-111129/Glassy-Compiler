#pragma once

#include "pch.h"
#include "Tokenizer.h"

namespace Glassy {

/*
    TODO:
    - parenthesis precendence
    - unary operators
    - variable expressions
*/

/*
    Grammar:

    program    -> statement*
    statement  -> ((identifier "=" expression) | (exit LIT)) ";"
    expression -> term (("+" | "-") term)*
    term       -> factor (("*" | "/") factor)*
    factor     -> LIT | IDENTIFIER | "(" expression ")"
*/

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual void print(std::ostream &out) const = 0;

    friend std::ostream &operator<<(std::ostream &os, const ASTNode &node) {
        node.print(os);
        return os;
    }
};

struct Expression : ASTNode {};

struct LiteralExpr : Expression {
    explicit LiteralExpr(double val) : value(val) {}

    void print(std::ostream &out) const override { out << value; }

    double value;
};

struct BinaryExpr : Expression {
    BinaryExpr(char op, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : op(op), left(std::move(left)), right(std::move(right)) {}

    void print(std::ostream &out) const override { out << *left << ' ' << op << ' ' << *right; }

    std::unique_ptr<Expression> left;
    char op;
    std::unique_ptr<Expression> right;
};

struct Statement : ASTNode {
    virtual void GenerateAsm(std::string &out) const = 0;
};

struct AssignStmt : Statement {
    AssignStmt(const std::string &name, std::unique_ptr<Expression> value)
        : identifier(name), value(std::move(value)) {}

    void GenerateAsm(std::string &out) const override { out += "; assign " + identifier + "\n"; }

    void print(std::ostream &out) const override { out << identifier << " = " << *value << ";"; }

    std::string identifier;
    std::unique_ptr<Expression> value;
};

struct DeclarStmt : Statement {
    DeclarStmt(const std::string &name) : identifier(name) {}

    void GenerateAsm(std::string &out) const override { out += "; declare " + identifier + "\n"; }

    void print(std::ostream &out) const override { out << identifier << ";"; }

    std::string identifier;
};

struct ExitStmt : Statement {
    ExitStmt(uint8_t value) : exitValue(value) {}

    void GenerateAsm(std::string &out) const override {
        out += "mov rax, 60\nmov rdi, " + std::to_string(exitValue) + "\nsyscall\n";
    }

    void print(std::ostream &out) const override {
        out << "exit " << static_cast<unsigned int>(exitValue) << ";";
    }

    uint8_t exitValue;
};

struct Program : ASTNode {
    void print(std::ostream &out) const override {
        out << "Program:\n";
        for (const auto &stmt : statements) {
            out << *stmt << '\n';
        }
    }

    std::vector<std::unique_ptr<Statement>> statements;
};

class Parser {
  public:
    explicit Parser(const std::vector<Token> &tokens);

    std::unique_ptr<Program> ParseProgram();

  private:
    std::unique_ptr<Expression> parseFactor();
    std::unique_ptr<Expression> parseTerm();
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Statement> parseStatement();

    [[nodiscard]] std::optional<Token> peek(const int offset = 0) const;
    Token consume();

    template <typename T, typename... Ts>
    std::optional<T> match(T type, Ts... rest);

    Token expect(TokenType type, const char *msg);

    const std::vector<Token> m_Tokens;
    size_t m_Index = 0;
};

template <typename T, typename... Ts>
inline std::optional<T> Parser::match(T first, Ts... rest) {
    auto tok = peek();
    if (!tok || !tok->subType) {
        return std::nullopt;
    }

    if (auto val = std::get_if<T>(&*tok->subType)) {
        if (((*val == first) || ... || (*val == rest))) {
            consume();
            return *val;
        }
    }
    return std::nullopt;
}

} // namespace Glassy
