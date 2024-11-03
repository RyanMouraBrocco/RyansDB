#pragma once

#include "vector"

const int PAGE_FREE_SPACE_PAGE_LENGTH = 7990;

class PageFreeSpacePageHeader
{
private:
    int m_nextPageOffSet;
    int m_previousPageOffSet;

public:
    PageFreeSpacePageHeader();

    int GetNextPageOffSet();
    int *GetNextPageOffSetPointer();
    void SetNextPageOffSet(int nextPageOffSet);
    int GetPreviousPageOffset();
    int *GetPreviousPageOffSetPointer();
    void SetPreviousPageOffSet(int previousPageOffSet);
};

class PageFreeSpacePage
{
private:
    PageFreeSpacePageHeader m_header;
    std::vector<unsigned char> m_freePages;

public:
    PageFreeSpacePage();
    PageFreeSpacePageHeader GetHeader();
    PageFreeSpacePageHeader &GetHeaderRef();
    void SetHeader(PageFreeSpacePageHeader header);
    int GetFreePageSize();
    unsigned char GetFreePage(int index);
    unsigned char *GetFreePagePointer(int index);
    void AddFreePageValue(unsigned char value);
    bool IsFull();
};