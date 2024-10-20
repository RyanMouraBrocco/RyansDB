#pragma once

#include <fstream>
#include <filesystem>
#include <string.h>
#include "../../../Contracts/Pages/PageFreeSpacePage/PageFreeSpacePage.hpp"
#include "./Header/PageFreeSpaceHeaderFileReader.hpp"

class PageFreeSpaceFileReader
{
private:
    const int m_firstPageOffSet = 96 + 8'000;
    std::ifstream &r_fileReader;
    std::shared_ptr<PageFreeSpacePage> p_pageFreeSpacePage;
    int m_currentPageOffSet = m_firstPageOffSet;

    void FetchFreePages();

public:
    PageFreeSpaceFileReader(std::ifstream &fileReader);
    PageFreeSpaceFileReader(std::ifstream &fileReader, int pageOffSet);
    PageFreeSpaceFileReader *LoadHeader();
    PageFreeSpaceFileReader *LoadFreePages();
    PageFreeSpaceFileReader *LoadAll();
    PageFreeSpacePage Extract();
};