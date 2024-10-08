#include "DataAccess.hpp"

std::optional<Error> DataAccess::CreateDatabaseFile(std::string name)
{
    if (m_databaseRepository.Exists(name))
        return Error(ErrorType::Unexpected, "Database already exists");

    int databaseId = 0;
    for (int i = 0; i < name.length(); i++)
    {
        databaseId += name[i];
    }

    DatabaseDefinition databaseDefinition = {
        DatabaseHeader(databaseId, name, sizeof(DatabaseDefinition)),
        BlockFreeSpacePage(),
        PageFreeSpacePage(),
        TableMappingPage(),
    };

    return m_databaseRepository.CreateDatabaseFile(databaseDefinition);
}

std::optional<Error> DataAccess::DropDatabaseFile(std::string name)
{
    if (!m_databaseRepository.Exists(name))
        return Error(ErrorType::Unexpected, "Database does not exist");

    return m_databaseRepository.DropDatabaseFile(name);
}