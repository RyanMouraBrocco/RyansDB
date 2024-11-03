#pragma once

#include "vector"

const int MAPPING_PAGE_TABLES_LENGTH = 998;

class MappingPageHeader
{
private:
    int m_nextPageOffSet;
    int m_previousPageOffSet;

public:
    MappingPageHeader();
    int GetNextPageOffSet();
    int *GetNextPageOffSetPointer();
    void SetNextPageOffSet(int nextPageOffSet);
    int GetPreviousPageOffSet();
    int *GetPreviousPageOffSetPointer();
    void SetPreviousPageOffSet(int previousPageOffSet);
};

class MappingPage
{
private:
    MappingPageHeader m_header;
    std::vector<int> m_tableIds;
    std::vector<int> m_tableOffSets;

public:
    MappingPage();
    MappingPageHeader GetHeader();
    MappingPageHeader &GetHeaderRef();
    void SetHeader(MappingPageHeader header);
    int GetTablesMapSize();
    int *GetTableIdPointerByIndex(int index);
    int *GetTableOffSetPointerByIndex(int index);
    void AddTableId(int tableId);
    void AddTableOffSet(int offSet);
    bool IsFull();
};