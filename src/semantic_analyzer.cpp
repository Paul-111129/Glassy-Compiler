#include "semantic_analyzer.h"
#include "symbol_table.h"

namespace Compiler {

SemanticAnalyzer::SemanticAnalyzer(Program* program, ScopeStack& scopes)
    : m_Program(program), m_Scopes(scopes) {}

void SemanticAnalyzer::Analyze() {}

} // namespace Compiler
