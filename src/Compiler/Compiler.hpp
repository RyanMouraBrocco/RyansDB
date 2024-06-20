#pragma once

#include "./LexycalAnalyzer/LexycalAnalyzer.hpp"
#include "./SyntaxAnalyzer/SyntaxAnalyzer.hpp"
#include <memory>

class Compiler
{
private:
    std::shared_ptr<SymbolTable> p_symbolTable;

public:
    Compiler();
    std::optional<Error> Build(std::string query);
};