#include "TableMappingPage.hpp"

TableMappingPageHeader::TableMappingPageHeader()
{
    m_startPageOffSet = -1;
    m_nextPageOffSet = -1;
    m_previousPageOffSet = -1;
}

TableMappingPageHeader::TableMappingPageHeader(int tableId) : m_tableId(tableId)
{
    m_startPageOffSet = -1;
    m_nextPageOffSet = -1;
    m_previousPageOffSet = -1;
}

int TableMappingPageHeader::GetTableId()
{
    return m_tableId;
}

int *TableMappingPageHeader::GetTableIdRef()
{
    return &m_tableId;
}

int TableMappingPageHeader::GetStartPageOffSet()
{
    return m_startPageOffSet;
}

int *TableMappingPageHeader::GetStartPageOffSetRef()
{
    return &m_startPageOffSet;
}

int TableMappingPageHeader::GetNextPageOffSet()
{
    return m_nextPageOffSet;
}

int *TableMappingPageHeader::GetNextPageOffSetRef()
{
    return &m_nextPageOffSet;
}

int TableMappingPageHeader::GetPreviousPageOffSet()
{
    return m_previousPageOffSet;
}

int *TableMappingPageHeader::GetPreviousPageOffSetRef()
{
    return &m_previousPageOffSet;
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

void TableMappingPage::SetTableBlockMap(unsigned int blockPosition, bool belongs)
{
    int bitsPerInt = (sizeof(int) * 8);
    unsigned int intPosition = blockPosition % bitsPerInt;
    unsigned int intIndex = blockPosition / bitsPerInt;

    if (intIndex >= m_tableBlockMap.size())
    {
        for (int i = m_tableBlockMap.size(); i < intIndex; i++)
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