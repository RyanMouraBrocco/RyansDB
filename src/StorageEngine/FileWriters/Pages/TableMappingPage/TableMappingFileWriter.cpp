#include "TableMappingFileWriter.hpp"

TableMappingFileWriter::TableMappingFileWriter(std::ofstream &fileWriter) : r_fileWriter(fileWriter)
{
}

TableMappingFileWriter::SetHeader(TableMappingPageHeader &header)
{
    r_fileWriter.seekp(, std::ios::beg);
    fileWriter.write(reinterpret_cast<char *>(header.GetNextPageOffSetRef()), sizeof(int));
    fileWriter.write(reinterpret_cast<char *>(header.GetPreviousPageOffSetRef()), sizeof(int));
}