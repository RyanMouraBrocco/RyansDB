#pragma once

#include <fstream>
#include <filesystem>
#include <string.h>
#include <memory>
#include "../../../../Contracts/Pages/MappingPage/MappingPage.hpp"

class MappingHeaderFileReader
{
private:
    std::ifstream &r_fileReader;
    std::shared_ptr<MappingPageHeader> p_mappingPageHeader;
    int m_currentPageOffSet;

    void FetchNextPageOffSet();
    void FetchPreviousPageOffSet();

public:
    MappingHeaderFileReader(std::ifstream &fileReader, int pageOffSet);
    MappingHeaderFileReader *LoadNextPageOffSet();
    MappingHeaderFileReader *LoadPreviousPageOffSet();
    MappingHeaderFileReader *LoadAll();
    MappingPageHeader Extract();
};