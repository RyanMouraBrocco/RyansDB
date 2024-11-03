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
    int *GetPageIdPointer();
    void SetPageId(int pageId);
    int GetPageLength();
    int *GetPageLengthPointer();
    void SetPageLength(int pageLength);
    int GetTableId();
    int *GetTableIdPointer();
    void SetTableId(int tableId);
};

class DataPageOffSet
{
private:
    std::vector<int> m_positions;

public:
    DataPageOffSet();
};

class DataPageData
{
private:
public:
};

class DataPage
{
private:
    DataPageHeader m_header;
    DataPageData m_data;
    DataPageOffSet m_offSet;

public:
    DataPage();
    DataPage(int pageId, int pageLength, int tableId);
    DataPageHeader GetHeader();
    DataPageHeader &GetHeaderRef();
};