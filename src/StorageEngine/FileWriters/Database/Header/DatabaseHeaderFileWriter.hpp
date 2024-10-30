#pragma once

#include <fstream>
#include <filesystem>
#include <string.h>
#include "../../../Contracts/Database/DatabaseDefinition.hpp"

class DatabaseHeaderFileWriter
{
private:
    std::fstream &r_fileWriter;

public:
    DatabaseHeaderFileWriter(std::fstream &fileWriter);
    void SetId(int *id);
    void SetDatabaseName(char *name);
    void SetFileLength(int *fileLength);
    void SetAll(DatabaseHeader &header);
};