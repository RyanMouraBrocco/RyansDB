#include "Compiler.hpp"

Compiler::Compiler() : p_symbolTable(std::make_shared<SymbolTable>())
{
}

std::variant<std::shared_ptr<ParserTreeNode>, Error> Compiler::Build(std::string query)
{
    auto lexycalAnalyzer = LexycalAnalyzer(p_symbolTable);
    auto lexycalResult = lexycalAnalyzer.Execute(query);
    if (lexycalResult.has_value())
        return lexycalResult.value();

    auto syntaxAnalyzer = SyntaxAnalyzer(p_symbolTable);
    auto syntaxResult = syntaxAnalyzer.Execute();

    auto semanticAnalyzer = SemanticAnalyzer(p_symbolTable);
    auto semanticAnalyzerResult = semanticAnalyzer.Execute();
    if (semanticAnalyzerResult.has_value())
        return semanticAnalyzerResult.value();

    return p_symbolTable->BuildTree();
}