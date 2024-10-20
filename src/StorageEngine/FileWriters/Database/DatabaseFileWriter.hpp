#pragma once

#include <fstream>
#include <filesystem>
#include <string.h>
#include "../../Contracts/Database/DatabaseDefinition.hpp"
#include "../../Contracts/Pages/MappingPage/MappingPage.hpp"
#include "../../Contracts/Pages/PageFreeSpacePage/PageFreeSpacePage.hpp"
#include "../Pages/MappingPage/MappingFileWriter.hpp"
#include "../Pages/PageFreeSpacePage/PageFreeSpaceFileWriter.hpp"
#include "./Header/DatabaseHeaderFileWriter.hpp"

class DatabaseFileWriter
{
private:
    std::ofstream &r_fileWriter;

public:
    DatabaseFileWriter(std::ofstream &fileWriter);
    void SetHeader(DatabaseHeader &header);
    void SetMapping(MappingPage &mapping);
    void SetPageFreeSpace(PageFreeSpacePage &pageFreeSpace);
    void SetAll(DatabaseDefinition &databaseDefinition);
};