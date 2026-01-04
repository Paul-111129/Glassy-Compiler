#pragma once

#include "pch.h"
#include "Parser.h"

namespace Glassy {

struct Generator {
  public:
    explicit Generator(Program* prog);
    std::string GenerateAsm();

  private:
    struct Variable {
        size_t stackLocation;
    };

    template <typename... Ts>
    struct overloaded : Ts... {
        using Ts::operator()...;
    };

    template <typename... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;

    void generateTerm(const Term* term);
    void generateExpression(const Expression* expr);
    void generateStatement(const Statement* stmt);

    void push(const std::string& reg) {
        m_Output += "push " + reg + "\n";
        m_StackSize++;
    }

    void pop(const std::string& reg) {
        m_Output += "pop " + reg + "\n";
        m_StackSize--;
    }

    Variable* lookup(const std::string& name) {
        for (auto it = m_Scopes.rbegin(); it != m_Scopes.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) {
                return &found->second;
            }
        }
        return nullptr;
    }

    const Program* m_Program;
    std::string m_Output;
    size_t m_StackSize = 0;

    // std::unordered_map<std::string, Variable> m_Variables;

    std::vector<std::unordered_map<std::string, Variable>> m_Scopes;
};

} // namespace Glassy
