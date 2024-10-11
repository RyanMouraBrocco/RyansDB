#pragma once

#include "../Error/error.hpp"
#include "../Compiler/SymbolTable/ParserTree/ParserTreeNode.hpp"
#include "../StorageEngine/DataAccess.hpp"
#include <optional>
#include <memory>
#include <variant>

struct TableColumnDefinition
{
    std::string columnName;
    TokenDefinition type;
    int typeLength;
    bool notNull = false;
    bool primaryKey = false;

    TableColumnDefinition(std::string columnName, TokenDefinition type, int typeLength = -1) : columnName(columnName), type(type), typeLength(typeLength)
    {
    }
};

class ExecuteEngine
{
private:
    DataAccess m_dataAccess;

    std::optional<Error> CreateDatabaseExecution(std::shared_ptr<ParserTreeNode> queryTree);
    std::optional<Error> DropDatabaseExecution(std::shared_ptr<ParserTreeNode> queryTree);
    std::optional<Error> CreateTableExecution(std::string databaseName, std::shared_ptr<ParserTreeNode> queryTree);
    std::variant<std::vector<TableColumnDefinition>, Error> ExtractColumns(std::shared_ptr<ParserTreeNode> queryTree);
    std::variant<std::tuple<TokenDefinition, int>, Error> ExtractColumnType(std::shared_ptr<ParserTreeNode> columnTypeTree);
    std::variant<std::tuple<bool, bool>, Error> ExtractColumnConstraints(std::shared_ptr<ParserTreeNode> columnConstrainsTree);

public:
    ExecuteEngine();
    std::optional<Error> Execute(std::shared_ptr<ParserTreeNode> queryTree);
};