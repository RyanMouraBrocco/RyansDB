#pragma once

#include "../Error/error.hpp"
#include "./Repositories/DatabaseRepository/DatabaseRepository.hpp"
#include "./Contracts/Database/DatabaseDefinition.hpp"
#include "optional"
#include "string"
#include <cstdlib>

class DataAccess
{
private:
    DatabaseRepository m_databaseRepository;

public:
    std::optional<Error>
    CreateDatabaseFile(std::string name);
};
