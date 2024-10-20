#pragma once

#include <fstream>
#include <filesystem>
#include <string.h>
#include "../../../../Contracts/Pages/PageFreeSpacePage/PageFreeSpacePage.hpp"

class PageFreeSpaceHeaderFileReader
{
private:
    std::ifstream &r_fileReader;
    std::shared_ptr<PageFreeSpacePageHeader> p_pageFreeSpacePageHeader;
    int m_currentPageOffSet;

    void FetchNextPageOffSet();
    void FetchPreviousPageOffSet();

public:
    PageFreeSpaceHeaderFileReader(std::ifstream &fileReader, int pageOffSet);
    PageFreeSpaceHeaderFileReader *LoadNextPageOffSet();
    PageFreeSpaceHeaderFileReader *LoadPreviousPageOffSet();
    PageFreeSpaceHeaderFileReader *LoadAll();
    PageFreeSpacePageHeader Extract();
};