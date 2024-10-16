#pragma once

#include "../Error/error.hpp"
#include "./Repositories/DatabaseRepository/DatabaseRepository.hpp"
#include "./Contracts/Database/DatabaseDefinition.hpp"
#include "./Contracts/Pages/MappingPage/MappingPage.hpp"
#include "./Contracts/Pages/TableMappingPage/TableMappingPage.hpp"
#include "optional"
#include "string"
#include <cstdlib>

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

class DataAccess
{
private:
    DatabaseRepository m_databaseRepository;

public:
    std::optional<Error> CreateDatabaseFile(std::string name);
    std::optional<Error> DropDatabaseFile(std::string name);
    std::optional<Error> CreateTableInDatabaseFile(std::string databaseName, std::string tableName, std::vector<TableColumnDefinition> columnsDefinitions);
};
