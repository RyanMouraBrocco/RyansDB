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

    DatabaseDefinition databaseDefinition;

    file.read(reinterpret_cast<char *>(&databaseDefinition), sizeof(databaseDefinition));

    file.close();

    return databaseDefinition;
}

std::optional<Error> DatabaseRepository::CreateDatabaseFile(DatabaseDefinition databaseDef)
{
    std::ofstream fileWriter(m_databasePath + "/" + databaseDef.header.databaseName + m_databaseExtension, std::ios::binary);

    if (!fileWriter.is_open())
        return Error(ErrorType::Unexpected, "Error to generate databasefile");

    int totalPageLength = 2 * m_pageSize + sizeof(databaseDef.header);
    fileWriter.write(reinterpret_cast<char *>(&databaseDef), sizeof(DatabaseDefinition));
    if (databaseDef.header.fileLength < totalPageLength)
    {
        fileWriter.seekp(totalPageLength - 1, std::ios::beg);
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

    if (!file.is_open())
        return Error(ErrorType::Unexpected, "Error to fetch databasefile");

    file.seekp(databaseDefinition.header.fileLength, std::ios::beg);

    file.write(reinterpret_cast<char *>(&tableMappingPage), sizeof(tableMappingPage));
    file.write(reinterpret_cast<char *>(dataPageBlock.get()), 8 * sizeof(DataPage));

    file.close();

    return std::nullopt;
}