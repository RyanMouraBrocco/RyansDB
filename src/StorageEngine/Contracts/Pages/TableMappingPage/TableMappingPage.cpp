#include "TableMappingPage.hpp"

TableMappingPageHeader::TableMappingPageHeader()
{
    m_startPageOffSet = -1;
    m_nextPageOffSet = -1;
    m_previousPageOffSet = -1;
}

TableMappingPage::TableMappingPage()
{
}

void TableMappingPage::SetTableBlockMap(int blockPosition, bool belongs)
{
    m_tableBlockMap.set(blockPosition, belongs);
}