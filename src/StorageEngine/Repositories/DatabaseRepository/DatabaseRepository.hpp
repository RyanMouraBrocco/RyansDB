#pragma once

#include "../../../Error/error.hpp"
#include "../../../Compiler/SymbolTable/SymbolTable.hpp"
#include "../../Contracts/Database/DatabaseDefinition.hpp"
#include "../../Contracts/Pages/DataPage.hpp"
#include "optional"
#include "memory"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string.h>

class DatabaseRepository
{
private:
    const std::string m_databasePath = "./database";
    const std::string m_databaseExtension = ".rdd";

public:
    DatabaseRepository();
    bool ExistsDatabase(std::string name);
    bool ExistsTableInDatabase(std::string databaseName, std::string tableName);
    std::optional<Error> CreateDatabaseFile(DatabaseDefinition databaseDef);
    std::optional<Error> DropDatabaseFile(std::string name);
    std::optional<Error> CreateTableInDatabaseFile(std::string databaseName, std::shared_ptr<DataPage> dataPageBlock);
};