#pragma once

#include "vector"

struct PageFreeSpacePageHeader
{
    int nextPageOffSet;
    int previousPageOffSet;

    PageFreeSpacePageHeader()
    {
        nextPageOffSet = -1;
        previousPageOffSet = -1;
    }
};

struct PageFreeSpacePage
{
    PageFreeSpacePageHeader header;
    unsigned char freePages[100];

    PageFreeSpacePage()
    {
        for (char i = 0; i < 100; i++)
        {
            freePages[i] = 0;
        }
    }
};