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
    int *GetNextPageOffSetRef();
    void SetNextPageOffSet(int nextPageOffSet);
    int GetPreviousPageOffSet();
    int *GetPreviousPageOffSetRef();
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
    void SetHeader(MappingPageHeader header);
    int GetTablesMapSize();
    int *GetTableIdRefByIndex(int index);
    int *GetTableOffSetRefByIndex(int index);
    void AddTableId(int tableId);
    void AddTableOffSet(int offSet);
    bool IsFull();
};