#pragma once

#include "bitset"

class BlockFreeSpacePageHeader
{
private:
    int m_nextPageOffSet;
    int m_previousPageOffSet;
};

class BlockFreeSpacePage
{
private:
    BlockFreeSpacePageHeader m_header;
    std::bitset<32> m_freeMappingBlocks;

public:
    BlockFreeSpacePage(BlockFreeSpacePageHeader header, std::bitset<32> freeMappingBlocks);
};