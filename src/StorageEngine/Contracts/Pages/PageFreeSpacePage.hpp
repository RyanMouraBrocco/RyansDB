#pragma once

#include "vector"

class PageFreeSpacePageHeader
{
private:
    int m_nextPageOffSet;
    int m_previousPageOffSet;
};

class PageFreeSpacePage
{
private:
    std::vector<unsigned char> m_freePages;
};