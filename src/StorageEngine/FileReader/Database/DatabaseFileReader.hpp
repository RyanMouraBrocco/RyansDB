#pragma once

#include <fstream>
#include <filesystem>
#include <string.h>
#include <memory>
#include "../../Contracts/Database/DatabaseDefinition.hpp"
#include "./Header/DatabaseHeaderFileReader.hpp"

class DatabaseFileReader
{
private:
    std::ifstream &r_fileReader;
    std::shared_ptr<DatabaseDefinition> p_databaseDefintion;

public:
    DatabaseFileReader(std::ifstream &r_fileReader);
    DatabaseFileReader *LoadHeader();
    DatabaseFileReader *LoadAll();
    DatabaseDefinition Extract();
};