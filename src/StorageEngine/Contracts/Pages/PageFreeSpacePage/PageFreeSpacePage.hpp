#pragma once

#include "vector"

const int PAGE_FREE_SPACE_PAGE_LENGTH = 7996;

class PageFreeSpacePageHeader
{
private:
    int m_nextPageOffSet;
    int m_previousPageOffSet;

public:
    PageFreeSpacePageHeader();

    int GetNextPageOffSet();
    int *GetNextPageOffSetRef();
    int GetPreviousPageOffset();
    int *GetPreviousPageOffsetRef();
};

class PageFreeSpacePage
{
private:
    PageFreeSpacePageHeader m_header;
    std::vector<unsigned char> m_freePages;

public:
    PageFreeSpacePage();
    PageFreeSpacePageHeader GetHeader();
    int GetFreePageSize();
    unsigned char GetFreePage(int index);
    unsigned char *GetFreePageRef(int index);
    void AddFreePageValue(unsigned char value);
    bool IsFull();
};