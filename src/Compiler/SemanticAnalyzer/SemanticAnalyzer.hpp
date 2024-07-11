#pragma once

#include <memory>
#include <optional>
#include <queue>
#include "../SymbolTable/SymbolTable.hpp"

class SemanticAnalyzer
{
private:
    std::shared_ptr<SymbolTable> p_symbolTable;

    std::vector<std::shared_ptr<ParserTreeNode>> SemanticAnalyzer::GetLogicalNodes();

public:
    SemanticAnalyzer(std::shared_ptr<SymbolTable> symbolTable);
    std::optional<Error> Execute();
};