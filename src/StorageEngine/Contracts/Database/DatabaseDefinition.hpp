#pragma once

#include "string"
#include "vector"

class DatabaseHeader
{
private:
    int m_id;
    std::string m_databaseName;
    int m_fileLength;

public:
    DatabaseHeader(int pageId, std::string databaseName, int fileLength);
    std::string GetDatabaseName();
    int m_fileLength;
};

class DatabaseFreeSpace
{
private:
    int m_firstPageOffSet;
    int m_lastPageOffSet;
};

class DatabaseTableMappings
{
private:
};

class DatabaseBlockSpace
{
private:
};

class DatabaseDefinition
{
private:
    DatabaseHeader m_header;
    DatabaseFreeSpace m_freeSpace;
    DatabaseTableMappings m_mappings;
    DatabaseBlockSpace m_blockSpace;

public:
    DatabaseDefinition(DatabaseHeader header, DatabaseFreeSpace freeSpace, DatabaseTableMappings tableMappings, DatabaseBlockSpace blockSpace);
    DatabaseHeader GetHeader();
    DatabaseFreeSpace GetFreeSpace();
    DatabaseTableMappings GetTableMappings();
    DatabaseBlockSpace GetBlockSpace();
};