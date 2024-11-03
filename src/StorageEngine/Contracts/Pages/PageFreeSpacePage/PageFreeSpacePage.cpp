#include "PageFreeSpacePage.hpp"

PageFreeSpacePageHeader::PageFreeSpacePageHeader()
{
    m_nextPageOffSet = -1;
    m_previousPageOffSet = -1;
}

int PageFreeSpacePageHeader::GetNextPageOffSet()
{
    return m_nextPageOffSet;
}

int *PageFreeSpacePageHeader::GetNextPageOffSetPointer()
{
    return &m_nextPageOffSet;
}

void PageFreeSpacePageHeader::SetNextPageOffSet(int nextPageOffSet)
{
    m_nextPageOffSet = nextPageOffSet;
}

int PageFreeSpacePageHeader::GetPreviousPageOffset()
{
    return m_previousPageOffSet;
}

int *PageFreeSpacePageHeader::GetPreviousPageOffSetPointer()
{
    return &m_previousPageOffSet;
}

void PageFreeSpacePageHeader::SetPreviousPageOffSet(int previousPageOffSet)
{
    m_previousPageOffSet = previousPageOffSet;
}

PageFreeSpacePage::PageFreeSpacePage()
{
}

PageFreeSpacePageHeader PageFreeSpacePage::GetHeader()
{
    return m_header;
}

PageFreeSpacePageHeader &PageFreeSpacePage::GetHeaderRef()
{
    return m_header;
}

void PageFreeSpacePage::SetHeader(PageFreeSpacePageHeader header)
{
    m_header = header;
}

int PageFreeSpacePage::GetFreePageSize()
{
    return m_freePages.size();
}

unsigned char PageFreeSpacePage::GetFreePage(int index)
{
    return m_freePages[index];
}

unsigned char *PageFreeSpacePage::GetFreePagePointer(int index)
{
    return &m_freePages[index];
}

bool PageFreeSpacePage::IsFull()
{
    return m_freePages.size() == PAGE_FREE_SPACE_PAGE_LENGTH;
}

void PageFreeSpacePage::AddFreePageValue(unsigned char value)
{
    m_freePages.push_back(value);
}