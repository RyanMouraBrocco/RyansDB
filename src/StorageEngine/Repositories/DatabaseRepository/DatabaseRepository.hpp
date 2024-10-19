#pragma once

#include "../../../Error/error.hpp"
#include "../../../Compiler/SymbolTable/SymbolTable.hpp"
#include "../../Contracts/Database/DatabaseDefinition.hpp"
#include "../../Contracts/Pages/DataPage/DataPage.hpp"
#include "../../Contracts/Pages/TableMappingPage/TableMappingPage.hpp"
#include "../../FileWriters/Database/Header/DatabaseHeaderFileWriter.hpp"
#include "../../FileWriters/Pages/MappingPage/MappingFileWriter.hpp"
#include "../../FileWriters/Pages/PageFreeSpacePage/PageFreeSpaceFileWriter.hpp"
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
    const int m_pageSizeInBytes = 8'000;
    const int m_databaseHeaderSizeInBytes = 96;

    std::variant<DatabaseDefinition, Error> GetDatabaseDefinition(std::string databaseName);

public:
    DatabaseRepository();
    bool ExistsDatabase(std::string name);
    bool ExistsTableInDatabase(std::string databaseName, std::string tableName);
    std::optional<Error> CreateDatabaseFile(DatabaseDefinition databaseDef);
    std::optional<Error> DropDatabaseFile(std::string name);
    std::optional<Error> CreateTableInDatabaseFile(std::string databaseName, TableMappingPage tableMappingPage, std::shared_ptr<DataPage> dataPageBlock);
};