#pragma once

#include "bitset"

struct BlockFreeSpacePageHeader
{
    int nextPageOffSet;
    int previousPageOffSet;

    BlockFreeSpacePageHeader()
    {
        nextPageOffSet = -1;
        previousPageOffSet = -1;
    }
};

struct BlockFreeSpacePage
{
    BlockFreeSpacePageHeader header;
    std::bitset<32> freeMappingBlocks;
};