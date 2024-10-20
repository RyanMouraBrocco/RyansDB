#pragma once

#include <fstream>
#include <filesystem>
#include <string.h>
#include <memory>
#include "../../../Contracts/Pages/MappingPage/MappingPage.hpp"
#include "./Header/MappingHeaderFileReader.hpp"

class MappingFileReader
{
private:
    const int m_firstPageOffSet = 96;
    std::ifstream &r_fileReader;
    std::shared_ptr<MappingPage> p_mappingPage;
    int m_currentPageOffSet = m_firstPageOffSet;

public:
    MappingFileReader(std::ifstream &fileReader);
    MappingFileReader(std::ifstream &fileReader, int pageOffSet);
    MappingFileReader *LoadHeader();
    MappingFileReader *LoadTableIds();
    MappingFileReader *LoadTableOffSets();
    MappingFileReader *LoadAll();
    MappingPage Extract();
};