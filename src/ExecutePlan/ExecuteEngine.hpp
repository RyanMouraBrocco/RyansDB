#pragma once

#include "../Error/error.hpp"
#include "../Compiler/SymbolTable/ParserTree/ParserTreeNode.hpp"
#include "../StorageEngine/DataAccess.hpp"
#include <optional>
#include <memory>

class ExecuteEngine
{
private:
    DataAccess m_dataAccess;

    std::optional<Error> CreateDatabaseExecution(std::shared_ptr<ParserTreeNode> queryTree);

public:
    ExecuteEngine();
    std::optional<Error> Execute(std::shared_ptr<ParserTreeNode> queryTree);
};