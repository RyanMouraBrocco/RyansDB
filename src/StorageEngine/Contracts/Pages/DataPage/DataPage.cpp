#include "DataPage.hpp"

DataPageHeader::DataPageHeader()
{
}

DataPageHeader::DataPageHeader(int pageId, int pageLength, int tableId) : m_pageId(pageId), m_pageLength(pageLength), m_tableId(tableId)
{
}

int DataPageHeader::GetPageId()
{
    return m_pageId;
}

int *DataPageHeader::GetPageIdRef()
{
    return &m_pageId;
}

int DataPageHeader::GetPageLength()
{
    return m_pageLength;
}

int *DataPageHeader::GetPageLengthRef()
{
    return &m_pageLength;
}

int DataPageHeader::GetTableId()
{
    return m_tableId;
}

int *DataPageHeader::GetTableIdRef()
{
    return &m_tableId;
}

DataPageOffSet::DataPageOffSet()
{
}

DataPage::DataPage()
{
}

DataPage::DataPage(int pageId, int pageLength, int tableId)
{
    m_header = DataPageHeader(pageId, pageLength, tableId);
}

DataPageHeader DataPage::GetHeader()
{
    return m_header;
}