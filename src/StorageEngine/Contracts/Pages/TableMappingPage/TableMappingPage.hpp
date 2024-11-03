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
    int *GetTableIdPointer();
    void SetTableId(int tableId);
    int GetStartPageOffSet();
    int *GetStartPageOffSetPointer();
    void SetStartPageOffSet(int startPageOffSet);
    int GetNextPageOffSet();
    int *GetNextPageOffSetPointer();
    void SetNextPageOffSet(int nextPageOffSet);
    int GetPreviousPageOffSet();
    int *GetPreviousPageOffSetPointer();
    void SetPreviousPageOffSet(int previousPageOffSet);
    int GetLastMappedPageIndex();
    int *GetLastMappedPageIndexPointer();
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
    TableMappingPageHeader &GetHeaderRef();
    void SetHeader(TableMappingPageHeader header);
    void SetTableBlockMap(unsigned int blockPosition, bool belongs);
    int GetTableBlockMapSize();
    unsigned int GetTableBlockMapByIndex(int index);
    unsigned int *GetTableBlockMapRefByIndex(int index);
    bool IsFull();
};