#pragma once

#include "vector"

const int TABLE_MAPPING_PAGE_LENGTH = 1996;

class TableMappingPageHeader
{
private:
    int m_tableId;
    int m_startPageOffSet;
    int m_nextPageOffSet;
    int m_previousPageOffSet;

public:
    TableMappingPageHeader();
    TableMappingPageHeader(int tableId);
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
    std::vector<unsigned int> m_tableBlockMap;

public:
    TableMappingPage();
    TableMappingPage(int tableId);
    TableMappingPageHeader GetHeader();
    void SetTableBlockMap(unsigned int blockPosition, bool belongs);
    int GetTableBlockMapSize();
    unsigned int GetTableBlockMapByIndex(int index);
    unsigned int *GetTableBlockMapRefByIndex(int index);
    bool IsFull();
};