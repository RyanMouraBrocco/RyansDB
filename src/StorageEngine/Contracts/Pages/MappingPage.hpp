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
    int tableIds[MAPPING_PAGE_TABLES_LENGTH];
    int tableOffSets[MAPPING_PAGE_TABLES_LENGTH];
};