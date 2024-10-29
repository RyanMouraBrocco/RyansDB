#include "TableMappingPage.hpp"

TableMappingPageHeader::TableMappingPageHeader()
{
    m_startPageOffSet = -1;
    m_nextPageOffSet = -1;
    m_previousPageOffSet = -1;
    m_lastMappedPageIndex = 0;
}

TableMappingPageHeader::TableMappingPageHeader(int tableId) : m_tableId(tableId)
{
    m_startPageOffSet = -1;
    m_nextPageOffSet = -1;
    m_previousPageOffSet = -1;
    m_lastMappedPageIndex = 0;
}

int TableMappingPageHeader::GetTableId()
{
    return m_tableId;
}

int *TableMappingPageHeader::GetTableIdRef()
{
    return &m_tableId;
}

void TableMappingPageHeader::SetTableId(int tableId)
{
    m_tableId = tableId;
}

int TableMappingPageHeader::GetStartPageOffSet()
{
    return m_startPageOffSet;
}

int *TableMappingPageHeader::GetStartPageOffSetRef()
{
    return &m_startPageOffSet;
}

void TableMappingPageHeader::SetStartPageOffSet(int startPageOffSet)
{
    m_startPageOffSet = startPageOffSet;
}

int TableMappingPageHeader::GetNextPageOffSet()
{
    return m_nextPageOffSet;
}

int *TableMappingPageHeader::GetNextPageOffSetRef()
{
    return &m_nextPageOffSet;
}

void TableMappingPageHeader::SetNextPageOffSet(int nextPageOffSet)
{
    m_nextPageOffSet = nextPageOffSet;
}

int TableMappingPageHeader::GetPreviousPageOffSet()
{
    return m_previousPageOffSet;
}

int *TableMappingPageHeader::GetPreviousPageOffSetRef()
{
    return &m_previousPageOffSet;
}

void TableMappingPageHeader::SetPreviousPageOffSet(int previousPageOffSet)
{
    m_previousPageOffSet = previousPageOffSet;
}

int TableMappingPageHeader::GetLastMappedPageIndex()
{
    return m_lastMappedPageIndex;
}

int *TableMappingPageHeader::GetLastMappedPageIndexRef()
{
    return &m_lastMappedPageIndex;
}

void TableMappingPageHeader::SetLastMappedPageIndex(int lastMappedPageIndex)
{
    m_lastMappedPageIndex = lastMappedPageIndex;
}

TableMappingPage::TableMappingPage()
{
}

TableMappingPage::TableMappingPage(int tableId)
{
    m_header = TableMappingPageHeader(tableId);
}

TableMappingPageHeader TableMappingPage::GetHeader()
{
    return m_header;
}

void TableMappingPage::SetHeader(TableMappingPageHeader header)
{
    m_header = header;
}

void TableMappingPage::SetTableBlockMap(unsigned int blockPosition, bool belongs)
{
    int bitsPerInt = (sizeof(int) * 8);
    unsigned int intPosition = blockPosition % bitsPerInt;
    unsigned int intIndex = blockPosition / bitsPerInt;

    if (intIndex >= m_tableBlockMap.size())
    {
        for (int i = m_tableBlockMap.size(); i <= intIndex; i++)
            m_tableBlockMap.push_back(0);
    }

    if (belongs)
        m_tableBlockMap[intIndex] |= 1 << intPosition;
    else
        m_tableBlockMap[intIndex] &= ~(1 << intPosition);
}

int TableMappingPage::GetTableBlockMapSize()
{
    return m_tableBlockMap.size();
}

unsigned int TableMappingPage::GetTableBlockMapByIndex(int index)
{
    return m_tableBlockMap[index];
}

unsigned int *TableMappingPage::GetTableBlockMapRefByIndex(int index)
{
    return &m_tableBlockMap[index];
}

bool TableMappingPage::IsFull()
{
    return m_tableBlockMap.size() == TABLE_MAPPING_PAGE_LENGTH;
}