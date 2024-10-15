#pragma once

#include "vector"

const int MAPPING_PAGE_TABLES_LENGTH = 1999;

struct MappingPageHeader
{
    int nextPageOffSet;
    int previousPageOffSet;

    MappingPageHeader()
    {
        nextPageOffSet = -1;
        previousPageOffSet = -1;
    }
};

struct MappingPage
{
    MappingPageHeader header;
    std::vector<int> tableIds;
    std::vector<int> tableOffSets;
};