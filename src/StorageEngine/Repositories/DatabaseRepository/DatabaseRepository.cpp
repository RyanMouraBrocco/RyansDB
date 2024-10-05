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

    DatabaseDefinition d2;

    std::ifstream file2(m_databasePath + "/" + databaseDef.header.databaseName + m_databaseExtension, std::ios::binary);

    if (!file2.is_open())
        return Error(ErrorType::Unexpected, "Error to generate databasefile");

    file2.read(reinterpret_cast<char *>(&d2), sizeof(d2));

    file2.close();

    return std::nullopt;
}

bool DatabaseRepository::Exists(std::string name)
{
    std::ifstream fileReader(m_databasePath + "/" + name + m_databaseExtension);
    return fileReader.good();
}