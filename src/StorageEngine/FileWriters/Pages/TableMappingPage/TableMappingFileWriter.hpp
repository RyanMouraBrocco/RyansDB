#pragma once;

#include <fstream>
#include <filesystem>
#include <string.h>
#include "../../../Contracts/Pages/TableMappingPage/TableMappingPage.hpp"

class TableMappingFileWriter
{
private:
    std::ofstream &r_fileWriter;

public:
    TableMappingFileWriter(std::ofstream &fileWriter);
    void SetHeader(TableMappingPageHeader &header);
    void SetAll(TableMappingPage &tableMappingPage);
};