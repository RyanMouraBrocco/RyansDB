#pragma once

#include "../Error/error.hpp"
#include "../Compiler/SymbolTable/ParserTree/ParserTreeNode.hpp"
#include <optional>
#include <memory>

class ExecuteEngine
{
public:
    ExecuteEngine();
    std::optional<Error> Execute(std::shared_ptr<ParserTreeNode> queryTree);
};