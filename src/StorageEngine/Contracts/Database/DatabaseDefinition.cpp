#include "DatabaseDefinition.hpp"

DatabaseHeader::DatabaseHeader()
{
}

DatabaseHeader::DatabaseHeader(int id, std::string databaseName, int fileLength) : m_id(id), m_fileLength(fileLength)
{
    auto length = databaseName.size() < 50 ? databaseName.size() : 49;
    int i = 0;
    for (; i < length; i++)
    {
        m_databaseName[i] = databaseName[i];
    }

    bool firstItemAfterFinishName = true;
    for (; i < 50; i++)
    {
        if (firstItemAfterFinishName)
        {
            m_databaseName[i] = '\0';
            firstItemAfterFinishName = false;
        }
        else
            m_databaseName[i] = ' ';
    }
}

int DatabaseHeader::GetId()
{
    return m_id;
}

int *DatabaseHeader::GetIdRef()
{
    return &m_id;
}

char *DatabaseHeader::GetDatabaseNameRef()
{
    return m_databaseName;
}

int DatabaseHeader::GetFileLength()
{
    return m_fileLength;
}

int *DatabaseHeader::GetFileLengthRef()
{
    return &m_fileLength;
}

DatabaseDefinition::DatabaseDefinition()
{
}

DatabaseDefinition::DatabaseDefinition(int id, std::string databaseName, int fileLength)
{
    m_header = DatabaseHeader(id, databaseName, fileLength);
}

DatabaseDefinition::DatabaseDefinition(DatabaseHeader header, MappingPage tableMapping, PageFreeSpacePage pageFreeSpace) : m_header(header), m_tableMapping(tableMapping), m_pageFreeSpace(pageFreeSpace)
{
}

DatabaseHeader DatabaseDefinition::GetHeader()
{
    return m_header;
}

void DatabaseDefinition::SetHeader(DatabaseHeader header)
{
    m_header = header;
}

MappingPage DatabaseDefinition::GetTableMappingPage()
{
    return m_tableMapping;
}

void DatabaseDefinition::SetTableMappingPage(MappingPage mappingPage)
{
    m_tableMapping = mappingPage;
}

PageFreeSpacePage DatabaseDefinition::GetPageFreeSpace()
{
    return m_pageFreeSpace;
}

void DatabaseDefinition::SetPageFreeSpace(PageFreeSpacePage pageFreeSpace)
{
    m_pageFreeSpace = pageFreeSpace;
}