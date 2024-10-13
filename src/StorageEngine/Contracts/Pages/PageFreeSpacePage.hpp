#pragma once

#include "vector"

const int PAGE_FREE_SPACE_PAGE_LENGTH = 7996;

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
    unsigned char freePages[PAGE_FREE_SPACE_PAGE_LENGTH];

    PageFreeSpacePage()
    {
        for (char i = 0; i < PAGE_FREE_SPACE_PAGE_LENGTH; i++)
        {
            freePages[i] = 0;
        }
    }
};