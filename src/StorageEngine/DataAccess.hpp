#pragma once

#include "../../Error/error.hpp"
#include "./Repositories/DatabaseRepository/DatabaseRepository.hpp"
#include "optional"
#include "string"

class DataAccess
{
private:
    DatabaseRepository m_databaseRepository;

public:
    std::optional<Error>
    CreateDatabaseFile(std::string name);
};
