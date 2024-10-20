#include "PageFreeSpaceHeaderFileReader.hpp"

PageFreeSpaceHeaderFileReader::PageFreeSpaceHeaderFileReader(std::ifstream &fileReader, int pageOffSet) : r_fileReader(fileReader), m_currentPageOffSet(pageOffSet)
{
    p_pageFreeSpacePageHeader = std::make_shared<PageFreeSpacePageHeader>();
}

void PageFreeSpaceHeaderFileReader::FetchNextPageOffSet()
{
    r_fileReader.read(reinterpret_cast<char *>(p_pageFreeSpacePageHeader->GetNextPageOffSetRef()), sizeof(int));
}

void PageFreeSpaceHeaderFileReader::FetchPreviousPageOffSet()
{
    r_fileReader.read(reinterpret_cast<char *>(p_pageFreeSpacePageHeader->GetPreviousPageOffsetRef()), sizeof(int));
}

PageFreeSpaceHeaderFileReader *PageFreeSpaceHeaderFileReader::LoadNextPageOffSet()
{
    r_fileReader.seekg(m_currentPageOffSet, std::ios::beg);
    FetchNextPageOffSet();
    return this;
}

PageFreeSpaceHeaderFileReader *PageFreeSpaceHeaderFileReader::LoadPreviousPageOffSet()
{
    r_fileReader.seekg(m_currentPageOffSet + sizeof(int), std::ios::beg);
    FetchPreviousPageOffSet();
    return this;
}

PageFreeSpaceHeaderFileReader *PageFreeSpaceHeaderFileReader::LoadAll()
{
    r_fileReader.seekg(m_currentPageOffSet, std::ios::beg);
    FetchNextPageOffSet();
    FetchPreviousPageOffSet();
    return this;
}

PageFreeSpacePageHeader PageFreeSpaceHeaderFileReader::Extract()
{
    return *p_pageFreeSpacePageHeader;
}
