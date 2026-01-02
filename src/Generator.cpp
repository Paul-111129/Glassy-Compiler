#include "Generator.h"
#include "Error.h"
#include <iostream>
#include <cassert>

namespace Glassy {

Generator::Generator(Program* prog) : m_Program(prog) {}

std::string Generator::GenerateAsm() {
    m_StackSize = 0;

    m_Output += "global _start\nsection .text\n_start:\n";

    for (const auto& stmt : m_Program->statements) {
        generateStatement(stmt);
    }

    m_Output += "mov rax, 60\nmov rdi, 0\nsyscall\n";
    return m_Output;
}

// clang-format off
void Generator::generateTerm(const Term* term) {
    assert(term);

    std::visit(overloaded{
        [&](const TermIdentifier* termIdent) {
            assert(termIdent);
             
            if (!m_Variables.contains(termIdent->identifier)) {
                Error("Undeclared variable '" + termIdent->identifier + "'");
            }
            const auto& var = m_Variables[termIdent->identifier];
            push("QWORD [rsp + " + std::to_string((m_StackSize - var.stackLocation - 1) * 8) + "]");
        },
        [&](const TermLiteral* termLiteral) {
            assert(termLiteral);

            m_Output += "mov rax, " + termLiteral->literal + "\n";
            push("rax");
        },
        [&](const TermParen* termParen) {
            assert(termParen);

            generateExpression(termParen->expr);
        }
    }, term->term);
}

void Generator::generateExpression(const Expression* expr) {
    assert(expr);

    std::visit(overloaded{
        [&](const Term* term) {
            generateTerm(term);
        },
        [&](const ExprBinary* exprBinary) {
            if (exprBinary->op == '+') {
                generateExpression(exprBinary->right);
                generateExpression(exprBinary->left);
                pop("rax");
                pop("rbx");
                m_Output += "add rax, rbx\n";
                push("rax");
            }
        }
    }, expr->expr);
}

void Generator::generateStatement(const Statement* stmt) {
    std::visit(overloaded{
        [&](const StmtExit* stmtExit) {
            generateExpression(stmtExit->expr);
            m_Output += "mov rax, 60\n";
            pop("rdi");
            m_Output += "syscall\n";
        },
        [&](const StmtLet* stmtLet) {
            if (m_Variables.contains(stmtLet->identifier)) {
                Error("Identifier already used: " + stmtLet->identifier);
            }
            generateExpression(stmtLet->expr);
            m_Variables.insert({ stmtLet->identifier, { m_StackSize - 1 } });
        },
        [&](const StmtAssign* stmtAssign) {
            if (!m_Variables.contains(stmtAssign->identifier)) {
                Error("Undeclared identifier: " + stmtAssign->identifier);
            }
            generateExpression(stmtAssign->expr);
            pop("rax");
            const auto& var = m_Variables[stmtAssign->identifier];
            m_Output += "mov [rsp + " + std::to_string((m_StackSize - var.stackLocation - 1) * 8) + "], rax\n";
        }
    }, stmt->stmt);
}

} // namespace Glassy
