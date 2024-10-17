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
    int GetTableId();
    int *GetTableIdRef();
    int GetStartPageOffSet();
    int *GetStartPageOffSetRef();
    int GetNextPageOffSet();
    int *GetNextPageOffSetRef();
    int GetPreviousPageOffSet();
    int *GetPreviousPageOffSetRef();
};

class TableMappingPage
{
private:
    TableMappingPageHeader m_header;
    std::bitset<7992> m_tableBlockMap;

public:
    TableMappingPage();
    TableMappingPageHeader GetHeader();
    void SetTableBlockMap(int blockPosition, bool belongs);
    std::bitset<7992> GetMap();
};