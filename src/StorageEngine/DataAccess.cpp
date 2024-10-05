#include "DataAccess.hpp"

std::optional<Error> DataAccess::CreateDatabaseFile(std::string name)
{
    if (m_databaseRepository.Exists(name))
        return Error(ErrorType::Unexpected, "Database already exists");

    DatabaseDefinition databaseDefinition = {
        DatabaseHeader(atoi(name.c_str()), name, sizeof(DatabaseDefinition)),
        BlockFreeSpacePage(),
        PageFreeSpacePage(),
        TableMappingPage(),
    };

    return m_databaseRepository.CreateDatabaseFile(databaseDefinition);
}