#include "MappingPage.hpp"

MappingPageHeader::MappingPageHeader()
{
    m_nextPageOffSet = -1;
    m_previousPageOffSet = -1;
}

int MappingPageHeader::GetNextPageOffSet()
{
    return m_nextPageOffSet;
}

int *MappingPageHeader::GetNextPageOffSetRef()
{
    return &m_nextPageOffSet;
}

int MappingPageHeader::GetPreviousPageOffSet()
{
    return m_previousPageOffSet;
}

int *MappingPageHeader::GetPreviousPageOffSetRef()
{
    return &m_previousPageOffSet;
}

MappingPage::MappingPage()
{
}

MappingPageHeader MappingPage::GetHeader()
{
    return m_header;
}

int MappingPage::GetTablesMapSize()
{
    return m_tableIds.size();
}

int *MappingPage::GetTableIdRefByIndex(int index)
{
    return &m_tableIds[index];
}

int *MappingPage::GetTableOffSetRefByIndex(int index)
{
    return &m_tableOffSets[index];
}

void MappingPage::AddTableOffSet(int tableId, int offSet)
{
    m_tableIds.push_back(tableId);
    m_tableOffSets.push_back(offSet);
}

bool MappingPage::IsFull()
{
    return m_tableIds.size() == MAPPING_PAGE_TABLES_LENGTH;
}