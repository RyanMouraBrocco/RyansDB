#pragma once

#include "string"
#include "vector"
#include "../Pages/BlockFreeSpacePage.hpp"
#include "../Pages/PageFreeSpacePage.hpp"
#include "../Pages/TableMappingPage.hpp"

struct DatabaseHeader
{
    int id;
    char databaseName[50];
    int fileLength;

    DatabaseHeader()
    {
    }

    DatabaseHeader(int id, std::string databaseName, int fileLength) : id(id), fileLength(fileLength)
    {
        auto length = databaseName.length();
        if (length > 50)
            length = 50;

        for (char i = 0; i < length; i++)
        {
            this->databaseName[i] = databaseName[i];
        }
    }
};

struct DatabaseDefinition
{
    DatabaseHeader header;
    BlockFreeSpacePage blockFreeSpace;
    PageFreeSpacePage pageFreeSpace;
    TableMappingPage tableMapping;

    DatabaseDefinition()
    {
    }

    DatabaseDefinition(DatabaseHeader header,
                       BlockFreeSpacePage blockFreeSpace,
                       PageFreeSpacePage pageFreeSpace,
                       TableMappingPage tableMapping) : header(header), blockFreeSpace(blockFreeSpace), pageFreeSpace(pageFreeSpace), tableMapping(tableMapping)
    {
    }
};