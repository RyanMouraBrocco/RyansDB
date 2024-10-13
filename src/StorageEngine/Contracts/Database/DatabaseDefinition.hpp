#pragma once

#include "string"
#include "vector"
#include "../Pages/PageFreeSpacePage.hpp"
#include "../Pages/MappingPage.hpp"

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
    MappingPage tableMapping;
    PageFreeSpacePage pageFreeSpace;

    DatabaseDefinition()
    {
    }

    DatabaseDefinition(DatabaseHeader header,
                       MappingPage tableMapping,
                       PageFreeSpacePage pageFreeSpace) : header(header), tableMapping(tableMapping), pageFreeSpace(pageFreeSpace)
    {
    }
};