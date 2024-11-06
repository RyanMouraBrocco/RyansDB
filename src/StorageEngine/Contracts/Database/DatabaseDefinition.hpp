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
    int *GetIdPointer();
    char *GetDatabaseNamePointer();
    int GetFileLength();
    int *GetFileLengthPointer();
};

class DatabaseDefinition
{
private:
    DatabaseHeader m_header;
    MappingPage m_tableMapping;
    PageFreeSpacePage m_pageFreeSpace;

public:
    DatabaseDefinition();
    DatabaseDefinition(int id, std::string databaseName, int fileLength);
    DatabaseDefinition(DatabaseHeader header, MappingPage tableMapping, PageFreeSpacePage pageFreeSpace);

    DatabaseHeader GetHeader();
    DatabaseHeader &GetHeaderRef();
    void SetHeader(DatabaseHeader header);
    MappingPage GetTableMappingPage();
    MappingPage &GetTableMappingPageRef();
    void SetTableMappingPage(MappingPage mappingPage);
    PageFreeSpacePage GetPageFreeSpace();
    PageFreeSpacePage &GetPageFreeSpaceRef();
    void SetPageFreeSpace(PageFreeSpacePage pageFreeSpace);
};