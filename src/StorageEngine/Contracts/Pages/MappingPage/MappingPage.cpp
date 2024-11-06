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

int *MappingPageHeader::GetNextPageOffSetPointer()
{
    return &m_nextPageOffSet;
}

void MappingPageHeader::SetNextPageOffSet(int nextPageOffSet)
{
    m_nextPageOffSet = nextPageOffSet;
}

int MappingPageHeader::GetPreviousPageOffSet()
{
    return m_previousPageOffSet;
}

int *MappingPageHeader::GetPreviousPageOffSetPointer()
{
    return &m_previousPageOffSet;
}

void MappingPageHeader::SetPreviousPageOffSet(int previousPageOffSet)
{
    m_previousPageOffSet = previousPageOffSet;
}

MappingPage::MappingPage()
{
}

MappingPageHeader MappingPage::GetHeader()
{
    return m_header;
}

MappingPageHeader &MappingPage::GetHeaderRef()
{
    return m_header;
}

void MappingPage::SetHeader(MappingPageHeader header)
{
    m_header = header;
}

int MappingPage::GetTablesMapSize()
{
    return m_tableIds.size();
}

int *MappingPage::GetTableIdPointerByIndex(int index)
{
    return &m_tableIds[index];
}

int MappingPage::GetTableIdByIndex(int index)
{
    return m_tableIds[index];
}

int *MappingPage::GetTableOffSetPointerByIndex(int index)
{
    return &m_tableOffSets[index];
}

void MappingPage::AddTableId(int tableId)
{
    m_tableIds.push_back(tableId);
}

void MappingPage::AddTableOffSet(int offSet)
{
    m_tableOffSets.push_back(offSet);
}

bool MappingPage::IsFull()
{
    return m_tableIds.size() == MAPPING_PAGE_TABLES_LENGTH;
}