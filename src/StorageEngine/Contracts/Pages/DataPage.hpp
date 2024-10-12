#pragma once

#include "vector"

struct DataPageHeader
{
    int m_pageId;
    int m_pageLength;
    int m_tableId;
};

struct DataPageOffSet
{
    std::vector<int> m_positions;
};

struct DataPage
{
    DataPageHeader m_header;
    DataPageOffSet m_offSet;

    DataPage()
    {
    }
};