#pragma once

#include "../../../Error/error.hpp"
#include "../../Contracts/Database/DatabaseDefinition.hpp"
#include "optional"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string.h>

class DatabaseRepository
{
private:
    const std::string m_databasePath = "./database";
    const std::string m_databaseExtension = ".rdd";

public:
    DatabaseRepository();
    bool Exists(std::string name);
    std::optional<Error> CreateDatabaseFile(DatabaseDefinition databaseDef);
    std::optional<Error> DropDatabaseFile(std::string name);
};