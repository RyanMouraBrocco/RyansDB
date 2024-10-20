#include "DatabaseRepository.hpp"

DatabaseRepository::DatabaseRepository()
{
    if (!std::filesystem::exists(m_databasePath))
        std::filesystem::create_directories(m_databasePath);
}

std::variant<DatabaseDefinition, Error> DatabaseRepository::GetDatabaseDefinition(std::string databaseName)
{
    std::ifstream file(m_databasePath + "/" + databaseName + m_databaseExtension, std::ios::binary);

    if (!file.is_open())
        return Error(ErrorType::Unexpected, "Error to fetch databasefile");

    DatabaseFileReader databaseFileReader(file);
    databaseFileReader.LoadAll();
    DatabaseDefinition database = databaseFileReader.Extract();

    file.close();

    return database;
}

std::optional<Error> DatabaseRepository::CreateDatabaseFile(DatabaseDefinition databaseDef)
{
    DatabaseHeader databaseHeader = databaseDef.GetHeader();
    std::ofstream fileWriter(m_databasePath + "/" + databaseHeader.GetDatabaseNameRef() + m_databaseExtension, std::ios::binary);

    if (!fileWriter.is_open())
        return Error(ErrorType::Unexpected, "Error to generate databasefile");

    DatabaseFileWriter databaseWriter(fileWriter);
    databaseWriter.SetAll(databaseDef);

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

    std::ofstream file(m_databasePath + "/" + databaseName + m_databaseExtension, std::ios::binary | std::ios::app);

    if (!file.is_open())
        return Error(ErrorType::Unexpected, "Error to fetch databasefile");

    auto lastPositionInTheFile = file.tellp();
    auto tableMappingHeader = tableMappingPage.GetHeader();
    file.write(reinterpret_cast<char *>(tableMappingHeader.GetTableIdRef()), sizeof(int));
    file.write(reinterpret_cast<char *>(tableMappingHeader.GetStartPageOffSetRef()), sizeof(int));
    file.write(reinterpret_cast<char *>(tableMappingHeader.GetNextPageOffSetRef()), sizeof(int));
    file.write(reinterpret_cast<char *>(tableMappingHeader.GetPreviousPageOffSetRef()), sizeof(int));

    auto map = tableMappingPage.GetMap().to_ullong();
    file.write(reinterpret_cast<char *>(&map), sizeof(int));

    for (int i = 0; i < 8; i++)
    {
        auto dataPage = dataPageBlock.get()[i];
        auto dataPageHeader = dataPage.GetHeader();
        file.write(reinterpret_cast<char *>(dataPageHeader.GetPageIdRef()), sizeof(int));
        file.write(reinterpret_cast<char *>(dataPageHeader.GetPageLengthRef()), sizeof(int));
        file.write(reinterpret_cast<char *>(dataPageHeader.GetTableIdRef()), sizeof(int));
        file.seekp(m_pageSizeInBytes - sizeof(DataPageHeader), std::ios::cur);
    }

    file.write("\0", 1);

    auto mapping = databaseDefinition.GetTableMappingPage();
    auto pageFreeSpace = databaseDefinition.GetPageFreeSapce();

    mapping.AddTableOffSet(tableMappingHeader.GetTableId(), lastPositionInTheFile);
    // write in file here

    pageFreeSpace.AddFreePageValue(0);
    // save in file here

    file.close();

    return std::nullopt;
}