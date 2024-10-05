#pragma once

#include "vector"

class TableMappingPageHeader
{
private:
    int m_nextPageOffSet;
    int m_previousPageOffSet;
};

class TableMappingPage
{
private:
    std::vector<int> m_tableMapping;
};