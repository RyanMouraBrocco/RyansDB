#pragma once

#include "vector"

const int TABLE_MAPPING_PAGE_LENGTH = 1995;

class TableMappingPageHeader
{
private:
    int m_tableId;
    int m_startPageOffSet;
    int m_nextPageOffSet;
    int m_previousPageOffSet;
    int m_lastMappedPageIndex;

public:
    TableMappingPageHeader();
    TableMappingPageHeader(int tableId);
    int GetTableId();
    int *GetTableIdRef();
    void SetTableId(int tableId);
    int GetStartPageOffSet();
    int *GetStartPageOffSetRef();
    void SetStartPageOffSet(int startPageOffSet);
    int GetNextPageOffSet();
    int *GetNextPageOffSetRef();
    void SetNextPageOffSet(int nextPageOffSet);
    int GetPreviousPageOffSet();
    int *GetPreviousPageOffSetRef();
    void SetPreviousPageOffSet(int previousPageOffSet);
    int GetLastMappedPageIndex();
    int *GetLastMappedPageIndexRef();
    void SetLastMappedPageIndex(int lastMappedPageIndex);
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
    void SetHeader(TableMappingPageHeader header);
    void SetTableBlockMap(unsigned int blockPosition, bool belongs);
    int GetTableBlockMapSize();
    unsigned int GetTableBlockMapByIndex(int index);
    unsigned int *GetTableBlockMapRefByIndex(int index);
    bool IsFull();
};