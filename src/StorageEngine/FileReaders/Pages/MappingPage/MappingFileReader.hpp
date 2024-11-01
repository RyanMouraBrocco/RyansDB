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
    std::fstream &r_fileReader;
    std::shared_ptr<MappingPage> p_mappingPage;
    int m_currentPageOffSet = m_firstPageOffSet;

public:
    MappingFileReader(std::fstream &fileReader);
    MappingFileReader(std::fstream &fileReader, int pageOffSet);
    MappingFileReader *LoadHeader();
    MappingFileReader *LoadTableIds();
    MappingFileReader *LoadTableOffSets();
    MappingFileReader *LoadAll();
    MappingPage Extract();
};