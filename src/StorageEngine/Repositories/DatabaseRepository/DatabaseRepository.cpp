#include "DatabaseRepository.hpp"

DatabaseRepository::DatabaseRepository()
{
    if (!std::filesystem::exists(m_databasePath))
        std::filesystem::create_directories(m_databasePath);
}

std::optional<Error> DatabaseRepository::CreateDatabaseFile(DatabaseDefinition databaseDef)
{
    std::ofstream fileWriter(m_databasePath + "/" + databaseDef.header.databaseName + m_databaseExtension, std::ios::binary);

    if (!fileWriter.is_open())
        return Error(ErrorType::Unexpected, "Error to generate databasefile");

    fileWriter.write(reinterpret_cast<char *>(&databaseDef), sizeof(databaseDef));

    fileWriter.close();

    return std::nullopt;
}

bool DatabaseRepository::Exists(std::string name)
{
    std::ifstream fileReader(m_databasePath + "/" + name + m_databaseExtension);
    return fileReader.good();
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