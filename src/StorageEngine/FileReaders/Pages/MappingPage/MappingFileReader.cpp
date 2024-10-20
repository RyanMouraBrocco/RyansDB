#include "MappingFileReader.hpp"

MappingFileReader::MappingFileReader(std::ifstream &fileReader) : r_fileReader(fileReader)
{
    p_mappingPage = std::make_shared<MappingPage>();
}

MappingFileReader::MappingFileReader(std::ifstream &fileReader, int pageOffSet) : r_fileReader(fileReader), m_currentPageOffSet(pageOffSet)
{
    p_mappingPage = std::make_shared<MappingPage>();
}

MappingFileReader *MappingFileReader::LoadHeader()
{
    MappingHeaderFileReader headerFileReader(r_fileReader, m_currentPageOffSet);
    auto header = headerFileReader.LoadAll()->Extract();
    p_mappingPage->SetHeader(header);
    return this;
}

MappingFileReader *MappingFileReader::LoadTableIds()
{
    r_fileReader.seekg(m_currentPageOffSet + sizeof(MappingPageHeader), std::ios::beg);

    short tablesMappingLength = 0;
    r_fileReader.read(reinterpret_cast<char *>(&tablesMappingLength), sizeof(short));

    for (short i = 0; i < tablesMappingLength; i++)
    {
        int value = 0;
        r_fileReader.read(reinterpret_cast<char *>(&value), sizeof(int));
        p_mappingPage->AddTableId(value);
    }

    return this;
}

MappingFileReader *MappingFileReader::LoadTableOffSets()
{
    r_fileReader.seekg(m_currentPageOffSet + sizeof(MappingPageHeader) + (MAPPING_PAGE_TABLES_LENGTH * sizeof(int)), std::ios::beg);

    short tablesMappingLength = 0;
    r_fileReader.read(reinterpret_cast<char *>(&tablesMappingLength), sizeof(short));

    for (short i = 0; i < tablesMappingLength; i++)
    {
        int value = 0;
        r_fileReader.read(reinterpret_cast<char *>(&value), sizeof(int));
        p_mappingPage->AddTableOffSet(value);
    }

    return this;
}

MappingFileReader *MappingFileReader::LoadAll()
{
    LoadHeader();
    LoadTableIds();
    LoadTableOffSets();
    return this;
}

MappingPage MappingFileReader::Extract()
{
    return *p_mappingPage;
}