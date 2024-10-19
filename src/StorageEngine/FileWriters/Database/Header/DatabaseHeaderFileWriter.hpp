#pragma once

#include <fstream>
#include <filesystem>
#include <string.h>
#include "../../../Contracts/Database/DatabaseDefinition.hpp"

class DatabaseHeaderFileWriter
{
private:
    std::ofstream &r_fileWriter;

public:
    DatabaseHeaderFileWriter(std::ofstream &fileWriter);
    void SetId(int *id);
    void SetDatabaseName(char *name);
    void SetFileLength(int *fileLength);
    void SetAll(DatabaseHeader &header);
};