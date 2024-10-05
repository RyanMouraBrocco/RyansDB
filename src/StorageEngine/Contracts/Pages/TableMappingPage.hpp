#pragma once

#include "vector"

struct TableMappingPageHeader
{
    int nextPageOffSet;
    int previousPageOffSet;

    TableMappingPageHeader()
    {
        nextPageOffSet = -1;
        previousPageOffSet = -1;
    }
};

struct TableMappingPage
{
    TableMappingPageHeader header;
    int tableMapping[100];
};