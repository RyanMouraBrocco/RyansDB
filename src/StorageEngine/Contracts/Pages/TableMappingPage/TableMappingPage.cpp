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

void TableMappingPage::SetTableBlockMap(int blockPosition, bool belongs)
{
    m_tableBlockMap.set(blockPosition, belongs);
}

std::bitset<7992> TableMappingPage::GetMap()
{
    return m_tableBlockMap;
}