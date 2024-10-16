#pragma once

#include "vector"
#include "bitset"

class TableMappingPageHeader
{
private:
    int m_tableId;
    int m_startPageOffSet;
    int m_nextPageOffSet;
    int m_previousPageOffSet;

public:
    TableMappingPageHeader();
};

class TableMappingPage
{
private:
    TableMappingPageHeader m_header;
    std::bitset<7992> m_tableBlockMap;

public:
    TableMappingPage();
    void SetTableBlockMap(int blockPosition, bool belongs);
};