#include "PageFreeSpaceFileReader.hpp"

PageFreeSpaceFileReader::PageFreeSpaceFileReader(std::ifstream &fileReader) : r_fileReader(fileReader)
{
    p_pageFreeSpacePage = std::make_shared<PageFreeSpacePage>();
}

PageFreeSpaceFileReader::PageFreeSpaceFileReader(std::ifstream &fileReader, int pageOffSet) : r_fileReader(fileReader), m_currentPageOffSet(pageOffSet)
{
    p_pageFreeSpacePage = std::make_shared<PageFreeSpacePage>();
}

void PageFreeSpaceFileReader::FetchFreePages()
{
    short freePageSize;
    r_fileReader.read(reinterpret_cast<char *>(&freePageSize), sizeof(short));
    for (short i = 0; i < freePageSize; i++)
    {
        unsigned char value;
        r_fileReader.read(reinterpret_cast<char *>(&value), sizeof(unsigned char));
        p_pageFreeSpacePage->AddFreePageValue(value);
    }
}

PageFreeSpaceFileReader *PageFreeSpaceFileReader::LoadHeader()
{
    PageFreeSpaceHeaderFileReader headerReader(r_fileReader, m_currentPageOffSet);
    auto header = headerReader.LoadAll()->Extract();
    p_pageFreeSpacePage->SetHeader(header);
    return this;
}

PageFreeSpaceFileReader *PageFreeSpaceFileReader::LoadFreePages()
{
    r_fileReader.seekg(m_currentPageOffSet + sizeof(PageFreeSpacePageHeader), std::ios::beg);
    FetchFreePages();
    return this;
}

PageFreeSpaceFileReader *PageFreeSpaceFileReader::LoadAll()
{
    LoadHeader();
    FetchFreePages();
    return this;
}

PageFreeSpacePage PageFreeSpaceFileReader::Extract()
{
    return *p_pageFreeSpacePage;
}