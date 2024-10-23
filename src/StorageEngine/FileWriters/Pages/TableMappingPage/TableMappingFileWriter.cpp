#include "TableMappingFileWriter.hpp"

TableMappingFileWriter::TableMappingFileWriter(std::ofstream &fileWriter) : r_fileWriter(fileWriter)
{
}

TableMappingFileWriter::TableMappingFileWriter(std::ofstream &fileWriter, int pageOffSet) : r_fileWriter(fileWriter), m_currentPageOffSet(pageOffSet)
{
}

void TableMappingFileWriter::WriteHeader(TableMappingPageHeader &header)
{
    r_fileWriter.write(reinterpret_cast<char *>(header.GetTableIdRef()), sizeof(int));
    r_fileWriter.write(reinterpret_cast<char *>(header.GetStartPageOffSetRef()), sizeof(int));
    r_fileWriter.write(reinterpret_cast<char *>(header.GetNextPageOffSetRef()), sizeof(int));
    r_fileWriter.write(reinterpret_cast<char *>(header.GetPreviousPageOffSetRef()), sizeof(int));
}

void TableMappingFileWriter::SetHeader(TableMappingPageHeader &header)
{
    r_fileWriter.seekp(m_currentPageOffSet, std::ios::beg);
    WriteHeader(header);
}

void TableMappingFileWriter::SetAll(TableMappingPage &tableMappingPage)
{
    r_fileWriter.seekp(m_currentPageOffSet, std::ios::beg);
    auto header = tableMappingPage.GetHeader();
    WriteHeader(header);

    auto tableBlockMapsLength = tableMappingPage.GetTableBlockMapSize();
    for (int i = 0; i < tableBlockMapsLength; i++)
    {
        r_fileWriter.write(reinterpret_cast<char *>(tableMappingPage.GetTableBlockMapRefByIndex(i)), sizeof(unsigned int));
    }

    if (!tableMappingPage.IsFull())
    {
        r_fileWriter.seekp(m_currentPageOffSet + 8'000 - 1, std::ios::beg);
        r_fileWriter.write("\0", 1);
    }
}