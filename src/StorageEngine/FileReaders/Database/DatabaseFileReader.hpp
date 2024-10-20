#pragma once

#include <fstream>
#include <filesystem>
#include <string.h>
#include <memory>
#include "../../Contracts/Database/DatabaseDefinition.hpp"
#include "./Header/DatabaseHeaderFileReader.hpp"
#include "../Pages/MappingPage/MappingFileReader.hpp"
#include "../Pages/PageSpaceFreePage/PageFreeSpaceFileReader.hpp"

class DatabaseFileReader
{
private:
    std::ifstream &r_fileReader;
    std::shared_ptr<DatabaseDefinition> p_databaseDefintion;

public:
    DatabaseFileReader(std::ifstream &r_fileReader);
    DatabaseFileReader *LoadHeader();
    DatabaseFileReader *LoadMappingPage();
    DatabaseFileReader *LoadPageFreeSpace();
    DatabaseFileReader *LoadAll();
    DatabaseDefinition Extract();
};