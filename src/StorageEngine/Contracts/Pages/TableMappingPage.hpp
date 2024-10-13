#pragma once

#include "vector"
#include "bitset"

struct TableMappingPageHeader
{
    int tableId;
    int startPageOffSet;
    int nextPageOffSet;
    int previousPageOffSet;

    TableMappingPageHeader()
    {
        tableId = 0;
        startPageOffSet = -1;
        nextPageOffSet = -1;
        previousPageOffSet = -1;
    }
};

struct TableMappingPage
{
    TableMappingPageHeader header;
    std::bitset<7992> tableBlockMap;
};