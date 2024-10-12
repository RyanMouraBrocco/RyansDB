#pragma once

#include "../Error/error.hpp"
#include "../Compiler/SymbolTable/ParserTree/ParserTreeNode.hpp"
#include "../StorageEngine/DataAccess.hpp"
#include "./Processors/DatabaseProcessor/DatabaseProcessor.hpp"
#include "./Processors/TableProcessor/TableProcessor.hpp"
#include <optional>
#include <memory>
#include <variant>

class ExecuteEngine
{
private:
    std::shared_ptr<DatabaseProcessor> p_databaseProcessor = DatabaseProcessor::GetInstance();
    std::shared_ptr<TableProcessor> p_tableProcessor = TableProcessor::GetInstance();

public:
    ExecuteEngine();
    std::optional<Error> Execute(std::shared_ptr<ParserTreeNode> queryTree);
};