#pragma once

#include "variant"
#include "optional"
#include "vector"
#include "memory"
#include "mutex"
#include "../../../../Error/error.hpp"
#include "../../../../StorageEngine/DataAccess.hpp"
#include "../../../../Compiler/SymbolTable/SymbolTable.hpp"

class CreateTableProcessor
{
private:
    static std::shared_ptr<CreateTableProcessor> p_singleton;
    static std::mutex m_mutex;

    DataAccess m_dataAccess;

    std::variant<std::vector<TableColumnDefinition>, Error> ExtractColumns(std::shared_ptr<ParserTreeNode> columnDefinitions);
    std::variant<std::string, Error> ExtractColumnName(std::shared_ptr<ParserTreeNode> columnNameTree);
    std::variant<std::tuple<TokenDefinition, int>, Error> ExtractColumnType(std::shared_ptr<ParserTreeNode> columnTypeTree);
    std::variant<std::tuple<bool, bool>, Error> ExtractColumnConstraints(std::shared_ptr<ParserTreeNode> columnConstraintsTree);
    std::optional<Error> ExtractColumnConstraint(std::shared_ptr<ParserTreeNode> columnConstraintTree, std::tuple<bool, bool> &constraintsTuple);

public:
    static std::shared_ptr<CreateTableProcessor> GetInstance();
    std::optional<Error> CreateTableExecution(std::string databaseName, std::shared_ptr<ParserTreeNode> queryTree);
};