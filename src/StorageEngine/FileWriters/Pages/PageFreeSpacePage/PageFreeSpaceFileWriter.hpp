#pragma once

#include <fstream>
#include <filesystem>
#include <string.h>
#include "../../../Contracts/Pages/PageFreeSpacePage/PageFreeSpacePage.hpp"

class PageFreeSpaceFileWriter
{
private:
    const int m_firstPageOffSet = 96 + 8'000;
    std::fstream &r_fileWriter;
    int m_currentPageOffSet = m_firstPageOffSet;

    void WriteHeader(PageFreeSpacePageHeader &header);

public:
    PageFreeSpaceFileWriter(std::fstream &fileWriter);
    PageFreeSpaceFileWriter(std::fstream &fileWriter, int pageOffSet);
    void SetHeader(PageFreeSpacePageHeader &header);
    void SetAll(PageFreeSpacePage &pageFreeSpace);
};