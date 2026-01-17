#pragma once

#include "parser.h"

namespace Compiler {

class ScopeStack;

class SemanticAnalyzer {
  public:
    SemanticAnalyzer(Program* program, ScopeStack& scopes);
    void Analyze();

  private:
    Program* m_Program;
    int64_t m_StackSize = 0;
    ScopeStack& m_Scopes;
};

} // namespace Compiler
