#include "DatabaseRepository.hpp"

DatabaseRepository::DatabaseRepository()
{
    if (!std::filesystem::exists(m_databasePath))
        std::filesystem::create_directories(m_databasePath);
}

std::variant<DatabaseDefinition, Error> DatabaseRepository::GetDatabaseDefinition(std::string databaseName)
{
    // std::ifstream file(m_databasePath + "/" + databaseName + m_databaseExtension, std::ios::binary);

    // if (!file.is_open())
    return Error(ErrorType::Unexpected, "Error to fetch databasefile");

    // DatabaseDefinition databaseDefinition;

    // file.read(reinterpret_cast<char *>(&databaseDefinition), sizeof(databaseDefinition));

    // file.close();

    // return databaseDefinition;
}

std::optional<Error> DatabaseRepository::CreateDatabaseFile(DatabaseDefinition databaseDef)
{
    DatabaseHeader databaseHeader = databaseDef.GetHeader();
    std::ofstream fileWriter(m_databasePath + "/" + databaseHeader.GetDatabaseNameRef() + m_databaseExtension, std::ios::binary);

    if (!fileWriter.is_open())
        return Error(ErrorType::Unexpected, "Error to generate databasefile");

    fileWriter.write(reinterpret_cast<char *>(databaseHeader.GetIdRef()), sizeof(int));
    fileWriter.write(databaseHeader.GetDatabaseNameRef(), 50 * sizeof(char));
    fileWriter.write(reinterpret_cast<char *>(databaseHeader.GetFileLengthRef()), sizeof(int));

    fileWriter.seekp(m_databaseHeaderSizeInBytes - 1, std::ios::beg);
    fileWriter.write("\0", 1);

    auto mappingPage = databaseDef.GetTableMappingPage();
    fileWriter.write(reinterpret_cast<char *>(mappingPage.GetHeader().GetNextPageOffSetRef()), sizeof(int));
    fileWriter.write(reinterpret_cast<char *>(mappingPage.GetHeader().GetPreviousPageOffSetRef()), sizeof(int));

    for (int i = 0; i < mappingPage.GetTablesMapSize(); i++)
    {
        fileWriter.write(reinterpret_cast<char *>(mappingPage.GetTableIdRefByIndex(i)), sizeof(int));
    }

    if (!mappingPage.IsFull())
    {
        fileWriter.seekp(m_databaseHeaderSizeInBytes + ((MAPPING_PAGE_TABLES_LENGTH + 2) * sizeof(int)) - 1, std::ios::beg);
        fileWriter.write("\0", 1);
    }

    for (int i = 0; i < mappingPage.GetTablesMapSize(); i++)
    {
        fileWriter.write(reinterpret_cast<char *>(mappingPage.GetTableOffSetRefByIndex(i)), sizeof(int));
    }

    if (!mappingPage.IsFull())
    {
        fileWriter.seekp(m_databaseHeaderSizeInBytes + m_pageSizeInBytes - 1, std::ios::beg);
        fileWriter.write("\0", 1);
    }

    auto pageFreeSpace = databaseDef.GetPageFreeSapce();
    fileWriter.write(reinterpret_cast<char *>(pageFreeSpace.GetHeader().GetNextPageOffSetRef()), sizeof(int));
    fileWriter.write(reinterpret_cast<char *>(pageFreeSpace.GetHeader().GetPreviousPageOffsetRef()), sizeof(int));

    for (int i = 0; i < pageFreeSpace.GetFreePageSize(); i++)
    {
        fileWriter.write(reinterpret_cast<char *>(pageFreeSpace.GetFreePageRef(i)), sizeof(unsigned char));
    }

    if (!pageFreeSpace.IsFull())
    {
        fileWriter.seekp(m_databaseHeaderSizeInBytes + (2 * m_pageSizeInBytes) - 1, std::ios::beg);
        fileWriter.write("\0", 1);
    }

    fileWriter.close();

    return std::nullopt;
}

bool DatabaseRepository::ExistsDatabase(std::string name)
{
    std::ifstream fileReader(m_databasePath + "/" + name + m_databaseExtension);
    return fileReader.good();
}

bool DatabaseRepository::ExistsTableInDatabase(std::string databaseName, std::string tableName)
{
    return false;
}

std::optional<Error> DatabaseRepository::DropDatabaseFile(std::string name)
{
    try
    {
        std::filesystem::remove(m_databasePath + "/" + name + m_databaseExtension);
        return std::nullopt;
    }
    catch (const std::filesystem::filesystem_error &err)
    {
        return Error(ErrorType::Unexpected, "Error to remove database file");
    }
}

std::optional<Error> DatabaseRepository::CreateTableInDatabaseFile(std::string databaseName, TableMappingPage tableMappingPage, std::shared_ptr<DataPage> dataPageBlock)
{
    auto databaseDefinitionResult = GetDatabaseDefinition(databaseName);
    if (std::holds_alternative<Error>(databaseDefinitionResult))
        return std::get<Error>(databaseDefinitionResult);

    auto databaseDefinition = std::get<DatabaseDefinition>(databaseDefinitionResult);

    std::ofstream file(m_databasePath + "/" + databaseName + m_databaseExtension, std::ios::binary);

    // if (!file.is_open())
    //     return Error(ErrorType::Unexpected, "Error to fetch databasefile");

    // file.seekp(databaseDefinition.header.fileLength, std::ios::beg);

    // file.write(reinterpret_cast<char *>(&tableMappingPage), sizeof(tableMappingPage));
    // file.write(reinterpret_cast<char *>(dataPageBlock.get()), 8 * sizeof(DataPage));

    // file.close();

    return std::nullopt;
}