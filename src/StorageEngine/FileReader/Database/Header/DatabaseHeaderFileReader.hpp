#pragma once

#include <fstream>
#include <filesystem>
#include <string.h>
#include <memory>
#include "../../../Contracts/Database/DatabaseDefinition.hpp"

class DatabaseHeaderFileReader
{
private:
    std::ifstream &r_fileReader;
    std::shared_ptr<DatabaseHeader> p_databaseHeader;

    void FetchId();
    void FetchDatabaseName();
    void FetchFileLength();

public:
    DatabaseHeaderFileReader(std::ifstream &r_fileReader);
    DatabaseHeaderFileReader *LoadId();
    DatabaseHeaderFileReader *LoadDatabaseName();
    DatabaseHeaderFileReader *LoadFileLength();
    DatabaseHeaderFileReader *LoadAll();
    DatabaseHeader Extract();
};