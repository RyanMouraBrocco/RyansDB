#pragma once

#include <memory>
#include <optional>
#include "../SymbolTable/SymbolTable.hpp"

class SemanticAnalyzer
{
private:
    std::shared_ptr<SymbolTable> p_symbolTable;

public:
    SemanticAnalyzer(std::shared_ptr<SymbolTable> symbolTable);
    std::optional<Error> Execute();
};