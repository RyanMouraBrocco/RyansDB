#pragma once

#include <fstream>
#include <filesystem>
#include <string.h>
#include "../../../Contracts/Pages/MappingPage/MappingPage.hpp"

class MappingFileWriter
{
private:
    const int m_firstPageOffSet = 96;
    std::ofstream &r_fileWriter;
    int m_currentPageOffSet = m_firstPageOffSet;

    void WriteHeader(MappingPageHeader &header);

public:
    MappingFileWriter(std::ofstream &fileWriter);
    MappingFileWriter(std::ofstream &fileWriter, int pageOffSet);
    void SetHeader(MappingPageHeader &header);
    void SetAll(MappingPage &mappingPage);
};