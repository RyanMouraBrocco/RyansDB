#pragma once

#include "vector"

class DataPageHeader
{
private:
    int m_pageId;
    int m_pageLength;
    int m_tableId;

public:
    DataPageHeader();
    DataPageHeader(int pageId, int pageLength, int tableId);
    int GetPageId();
    int *GetPageIdRef();
    int GetPageLength();
    int *GetPageLengthRef();
    int GetTableId();
    int *GetTableIdRef();
};

class DataPageOffSet
{
private:
    std::vector<int> m_positions;

public:
    DataPageOffSet();
};

class DataPage
{
private:
    DataPageHeader m_header;
    DataPageOffSet m_offSet;

public:
    DataPage();
    DataPage(int pageId, int pageLength, int tableId);
    DataPageHeader GetHeader();
};