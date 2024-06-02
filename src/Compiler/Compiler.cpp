#include "Compiler.hpp"

Compiler::Compiler() : m_lexicalAnalyzer(std::make_shared<SymbolTable>())
{
}

std::optional<Error> Compiler::Build(std::string query)
{
    return m_lexicalAnalyzer.Execute(query);
}