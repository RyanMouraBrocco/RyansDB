#pragma once

#include "./LexycalAnalyzer/LexycalAnalyzer.hpp"
#include "./SyntaxAnalyzer/SyntaxAnalyzer.hpp"
#include "./SemanticAnalyzer/SemanticAnalyzer.hpp"
#include <memory>

class Compiler
{
private:
    std::shared_ptr<SymbolTable> p_symbolTable;

public:
    Compiler();
    std::variant<std::shared_ptr<ParserTreeNode>, Error> Build(std::string query);
};