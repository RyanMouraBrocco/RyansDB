#pragma once

#include "vector"

const int MAPPING_PAGE_TABLES_LENGTH = 1999;

class MappingPageHeader
{
private:
    int m_nextPageOffSet;
    int m_previousPageOffSet;

public:
    MappingPageHeader();
    int GetNextPageOffSet();
    int *GetNextPageOffSetRef();
    int GetPreviousPageOffSet();
    int *GetPreviousPageOffSetRef();
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
    int GetTablesMapSize();
    int *GetTableIdRefByIndex(int index);
    int *GetTableOffSetRefByIndex(int index);
    void AddTableOffSet(int tableId, int offSet);
    bool IsFull();
};