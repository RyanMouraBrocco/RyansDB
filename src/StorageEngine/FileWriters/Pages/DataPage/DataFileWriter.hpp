#pragma once

#include <fstream>
#include <filesystem>
#include <string.h>
#include "../../../Contracts/Pages/DataPage/DataPage.hpp"

class DataFileWriter
{
private:
    const int m_firstPageOffSet = 96 + 24'000;
    std::ofstream &r_fileWriter;
    int m_currentPageOffSet = m_firstPageOffSet;

    void WriteHeader(DataPageHeader &header);

public:
    DataFileWriter(std::ofstream &fileWriter);
    DataFileWriter(std::ofstream &fileWriter, int pageOffSet);
    void SetHeader(DataPageHeader &header);
    void SetAll(DataPage &dataPage);
};