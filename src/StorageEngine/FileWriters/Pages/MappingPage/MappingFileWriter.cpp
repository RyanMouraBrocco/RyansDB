#include "MappingFileWriter.hpp"

MappingFileWriter::MappingFileWriter(std::ofstream &fileWriter) : r_fileWriter(fileWriter)
{
}

MappingFileWriter::MappingFileWriter(std::ofstream &fileWriter, int pageOffSet) : r_fileWriter(fileWriter), m_currentPageOffSet(pageOffSet)
{
}

void MappingFileWriter::WriteHeader(MappingPageHeader &header)
{
    r_fileWriter.write(reinterpret_cast<char *>(header.GetNextPageOffSetRef()), sizeof(int));
    r_fileWriter.write(reinterpret_cast<char *>(header.GetPreviousPageOffSetRef()), sizeof(int));
}

void MappingFileWriter::SetHeader(MappingPageHeader &header)
{
    r_fileWriter.seekp(m_currentPageOffSet, std::ios::beg);
    WriteHeader(header);
}

void MappingFileWriter::SetAll(MappingPage &mappingPage)
{
    r_fileWriter.seekp(m_currentPageOffSet, std::ios::beg);
    auto header = mappingPage.GetHeader();
    WriteHeader(header);

    short tableIdsLength = (short)mappingPage.GetTablesMapSize();
    r_fileWriter.write(reinterpret_cast<char *>(&tableIdsLength), sizeof(short));

    for (short i = 0; i < tableIdsLength; i++)
    {
        r_fileWriter.write(reinterpret_cast<char *>(mappingPage.GetTableIdRefByIndex(i)), sizeof(int));
    }

    if (!mappingPage.IsFull())
    {
        r_fileWriter.seekp(m_currentPageOffSet + (MAPPING_PAGE_TABLES_LENGTH * sizeof(int)) - 1, std::ios::beg);
        r_fileWriter.write("\0", 1);
    }

    r_fileWriter.write(reinterpret_cast<char *>(&tableIdsLength), sizeof(short));

    for (short i = 0; i < tableIdsLength; i++)
    {
        r_fileWriter.write(reinterpret_cast<char *>(mappingPage.GetTableOffSetRefByIndex(i)), sizeof(int));
    }

    if (!mappingPage.IsFull())
    {
        r_fileWriter.seekp(m_currentPageOffSet + 8'000 - 1, std::ios::beg);
        r_fileWriter.write("\0", 1);
    }
}