#pragma once

#include "../../../Error/error.hpp"
#include "../../../Compiler/SymbolTable/SymbolTable.hpp"
#include "../../Contracts/Database/DatabaseDefinition.hpp"
#include "../../Contracts/Pages/DataPage/DataPage.hpp"
#include "../../Contracts/Pages/TableMappingPage/TableMappingPage.hpp"
#include "../../FileWriters/Database/DatabaseFileWriter.hpp"
#include "../../FileWriters/Pages/TableMappingPage/TableMappingFileWriter.hpp"
#include "../../FileWriters/Pages/DataPage/DataFileWriter.hpp"
#include "../../FileReaders/Database/DatabaseFileReader.hpp"
#include "optional"
#include "memory"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string.h>
#include <tuple>

class DatabaseRepository
{
private:
    const std::string m_databasePath = "./database";
    const std::string m_databaseExtension = ".rdd";
    const int m_pageSizeInBytes = 8'000;
    const int m_databaseHeaderSizeInBytes = 96;

    std::variant<DatabaseDefinition, Error> GetDatabaseDefinition(std::string databaseName);

    void AddTableInMapping(std::fstream &file, DatabaseDefinition &databaseDefinition, int tableId, int tableOffSet);
    std::tuple<int, MappingPage> CreateIfNotExistsLastMappingPage(std::fstream &file, DatabaseDefinition &databaseDefinition);
    void AddPageFreeSpaceForANewTable(std::fstream &file, DatabaseDefinition &databaseDefinition);
    std::tuple<int, PageFreeSpacePage> CreateIfNotExistsLastPageFreeSpacePage(std::fstream &file, DatabaseDefinition &databaseDefinition);
    std::tuple<int, PageFreeSpacePage> CreateInBackNewPageFreeSpace(std::fstream &file, PageFreeSpacePage &currentPage, int currentPageOffSet);
    void AddTableMappingPage(std::fstream &file, TableMappingPage tableMappingPage, int tableMappingStartPosition);

public:
    DatabaseRepository();
    bool ExistsDatabase(std::string name);
    std::variant<bool, Error> ExistsTableInDatabase(std::string databaseName, int tableId);
    std::optional<Error> CreateDatabaseFile(DatabaseDefinition databaseDef);
    std::optional<Error> DropDatabaseFile(std::string name);
    std::optional<Error> CreateTableInDatabaseFile(std::string databaseName, TableMappingPage tableMappingPage, std::shared_ptr<DataPage> dataPageBlock);
};