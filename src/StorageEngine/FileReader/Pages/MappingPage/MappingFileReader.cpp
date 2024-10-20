#include "MappingFileReader.hpp"

MappingFileReader::MappingFileReader(std::ifstream &fileReader) : r_fileReader(fileReader)
{
}

MappingFileReader::MappingFileReader(std::ifstream &fileReader, int pageOffSet) : r_fileReader(fileReader), m_currentPageOffSet(pageOffSet)
{
}

MappingFileReader *MappingFileReader::LoadHeader()
{
    MappingHeaderFileReader headerFileReader(r_fileReader, m_currentPageOffSet);
    auto header = headerFileReader.LoadAll();
    p_mappingPage->SetHeader(header);
    return this;
}

MappingFileReader *MappingFileReader::LoadTableIds()
{
}

MappingFileReader *MappingFileReader::LoadTableOffSets()
{
}

MappingFileReader *MappingFileReader::LoadAll()
{
}

MappingPage MappingFileReader::Extract()
{
}