#include "DataFileWriter.hpp"

DataFileWriter::DataFileWriter(std::fstream &fileWriter) : r_fileWriter(fileWriter)
{
}

DataFileWriter::DataFileWriter(std::fstream &fileWriter, int pageOffSet) : r_fileWriter(fileWriter), m_currentPageOffSet(pageOffSet)
{
}

void DataFileWriter::WriteHeader(DataPageHeader &header)
{
    r_fileWriter.write(reinterpret_cast<char *>(header.GetPageIdRef()), sizeof(int));
    r_fileWriter.write(reinterpret_cast<char *>(header.GetPageLengthRef()), sizeof(int));
    r_fileWriter.write(reinterpret_cast<char *>(header.GetTableIdRef()), sizeof(int));
}

void DataFileWriter::SetHeader(DataPageHeader &header)
{
    r_fileWriter.seekp(m_currentPageOffSet, std::ios::beg);
    WriteHeader(header);
}

void DataFileWriter::SetAll(DataPage &dataPage)
{
    r_fileWriter.seekp(m_currentPageOffSet, std::ios::beg);
    auto header = dataPage.GetHeader();
    WriteHeader(header);

    r_fileWriter.seekp(m_currentPageOffSet + 8'000 - 1, std::ios::beg);
    r_fileWriter.write("\0", 1);
}