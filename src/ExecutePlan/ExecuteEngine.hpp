#pragma once

#include "../Error/error.hpp"
#include "../Compiler/SymbolTable/ParserTree/ParserTreeNode.hpp"
#include "../StorageEngine/DataAccess.hpp"
#include <optional>
#include <memory>

struct TableColumnDefinition
{
    std::string columnName;
    Token type;
    int typeLength;
    std::vector<Token> constraints;

    TableColumnDefinition(std::string columnName, Token type, int typeLength = -1) : columnName(columnName), type(type), typeLength(typeLength)
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

public:
    ExecuteEngine();
    std::optional<Error> Execute(std::shared_ptr<ParserTreeNode> queryTree);
};