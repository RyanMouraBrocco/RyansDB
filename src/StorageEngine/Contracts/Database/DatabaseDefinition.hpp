#pragma once

#include "string"
#include "vector"
#include "../Pages/PageFreeSpacePage/PageFreeSpacePage.hpp"
#include "../Pages/MappingPage/MappingPage.hpp"

class DatabaseHeader
{
private:
    int m_id;
    char m_databaseName[50];
    int m_fileLength;

public:
    DatabaseHeader();
    DatabaseHeader(int id, std::string databaseName, int fileLength);

    int GetId();
    int *GetIdRef();
    char *GetDatabaseNameRef();
    int GetFileLength();
    int *GetFileLengthRef();
};

class DatabaseDefinition
{
private:
    DatabaseHeader m_header;
    MappingPage m_tableMapping;
    PageFreeSpacePage m_pageFreeSpace;

public:
    DatabaseDefinition(int id, std::string databaseName, int fileLength);
    DatabaseDefinition(DatabaseHeader header, MappingPage tableMapping, PageFreeSpacePage pageFreeSpace);

    DatabaseHeader GetHeader();
    MappingPage GetTableMappingPage();
    PageFreeSpacePage GetPageFreeSapce();
};