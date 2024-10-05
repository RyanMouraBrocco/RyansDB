#include "DataAccess.hpp"

std::optional<Error> DataAccess::CreateDatabaseFile(std::string name)
{
    if (m_databaseRepository.Exists(name))
        return Error(ErrorType::, "Database already exists");

    return m_databaseRepository.CreateNewDatabaseFile(name);
}