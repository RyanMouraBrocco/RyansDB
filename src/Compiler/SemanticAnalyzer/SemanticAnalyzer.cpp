#include "SemanticAnalyzer.hpp"

SemanticAnalyzer::SemanticAnalyzer(std::shared_ptr<SymbolTable> symbolTable) : p_symbolTable(symbolTable)
{
}

std::optional<Error> SemanticAnalyzer::Execute()
{
    
    return std::nullopt;
}