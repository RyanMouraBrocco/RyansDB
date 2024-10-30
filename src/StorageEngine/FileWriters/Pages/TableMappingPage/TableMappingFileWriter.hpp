#pragma once

#include <fstream>
#include <filesystem>
#include <string.h>
#include "../../../Contracts/Pages/TableMappingPage/TableMappingPage.hpp"

class TableMappingFileWriter
{
private:
    const int m_firstPageOffSet = 96 + 16'000;
    std::fstream &r_fileWriter;
    int m_currentPageOffSet = m_firstPageOffSet;

    void WriteHeader(TableMappingPageHeader &header);

public:
    TableMappingFileWriter(std::fstream &fileWriter);
    TableMappingFileWriter(std::fstream &fileWriter, int pageOffSet);
    void SetHeader(TableMappingPageHeader &header);
    void SetAll(TableMappingPage &tableMappingPage);
};