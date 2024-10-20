#include "MappingHeaderFileReader.hpp"

MappingHeaderFileReader::MappingHeaderFileReader(std::ifstream &fileReader, int pageOffSet) : r_fileReader(fileReader), m_currentPageOffSet(pageOffSet)
{
    p_mappingPageHeader = std::make_shared<MappingPageHeader>();
}

void MappingHeaderFileReader::FetchNextPageOffSet()
{
    r_fileReader.read(reinterpret_cast<char *>(p_mappingPageHeader->GetNextPageOffSetRef()), sizeof(int));
}

void MappingHeaderFileReader::FetchPreviousPageOffSet()
{
    r_fileReader.read(reinterpret_cast<char *>(p_mappingPageHeader->GetPreviousPageOffSetRef()), sizeof(int));
}

MappingHeaderFileReader *MappingHeaderFileReader::LoadNextPageOffSet()
{
    r_fileReader.seekg(m_currentPageOffSet, std::ios::beg);
    FetchNextPageOffSet();
    return this;
}

MappingHeaderFileReader *MappingHeaderFileReader::LoadPreviousPageOffSet()
{
    r_fileReader.seekg(m_currentPageOffSet + sizeof(int), std::ios::beg);
    FetchPreviousPageOffSet();
    return this;
}

MappingHeaderFileReader *MappingHeaderFileReader::LoadAll()
{
    r_fileReader.seekg(m_currentPageOffSet, std::ios::beg);
    FetchNextPageOffSet();
    FetchPreviousPageOffSet();
    return this;
}

MappingPageHeader MappingHeaderFileReader::Extract()
{
    return *p_mappingPageHeader;
}
