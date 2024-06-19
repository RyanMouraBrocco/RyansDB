#include "Compiler.hpp"

Compiler::Compiler() : p_symbolTable(std::make_shared<SymbolTable>())
{
}

std::optional<Error> Compiler::Build(std::string query)
{
    auto lexycalAnalyzer = LexycalAnalyzer(p_symbolTable);
    auto lexycalResult = lexycalAnalyzer.Execute(query);
    if (lexycalResult.has_value())
        return lexycalResult;

    auto syntaxAnalyzer = SyntaxAnalyzer(p_symbolTable);
    auto syntaxResult = syntaxAnalyzer.Execute();
}