#pragma once

#include "vector"

class DataPageHeader
{
private:
    int m_pageId;
    int m_pageLength;
    int m_tableId;
};

class DataPageOffSet
{
private:
    std::vector<int> m_positions;
};

class DataPage
{
private:
    DataPageHeader m_header;
    DataPageOffSet m_offSet;

    DataPage(DataPageHeader header, DataPageOffSet offSet);
};