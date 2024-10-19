#pragma once

#include <fstream>
#include <filesystem>
#include <string.h>
#include "../../../Contracts/Pages/PageFreeSpacePage/PageFreeSpacePage.hpp"

class PageFreeSpaceFileWriter
{
private:
    const int m_firstPageOffSet = 96 + 8'000;
    std::ofstream &r_fileWriter;
    int m_currentPageOffSet = m_firstPageOffSet;

    void WriteHeader(PageFreeSpacePageHeader &header);

public:
    PageFreeSpaceFileWriter(std::ofstream &fileWriter);
    PageFreeSpaceFileWriter(std::ofstream &fileWriter, int pageOffSet);
    void SetHeader(PageFreeSpacePageHeader &header);
    void SetAll(PageFreeSpacePage &pageFreeSpace);
};