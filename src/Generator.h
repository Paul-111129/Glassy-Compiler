#pragma once

#include "Parser.h"

namespace Glassy {

struct Generator : public AstVisitor {
  public:
    explicit Generator(std::unique_ptr<Program> prog) : program(std::move(prog)) {}

    std::string GenerateAsm() {
        program->accept(*this);
        return output;
    }

  private:
    void visit(const LiteralExpr& node) override {}

    void visit(const BinaryExpr& node) override {}

    void visit(const AssignStmt& stmt) override {
        stmt.expr->accept(*this);
        output += "; assign " + std::to_string(stmt.expr->value) + " to " + stmt.identifier + "\n";
    }

    void visit(const DeclarStmt& stmt) override { output += "; declare " + stmt.identifier + "\n"; }

    void visit(const ExitStmt& stmt) override {
        stmt.expr->accept(*this);
        output +=
            "mov rax, 60\nmov rdi, " + std::to_string(static_cast<uint8_t>(stmt.expr->value)) + "\nsyscall\n";
    }

    void visit(const Program& prog) override {
        output += "section .text\nglobal _start\n_start:\n";
        for (const auto& stmt : prog.statements) {
            stmt->accept(*this);
        }
    }

    const std::unique_ptr<Program> program;
    std::string output;
    int label_counter = 0;
};

} // namespace Glassy
