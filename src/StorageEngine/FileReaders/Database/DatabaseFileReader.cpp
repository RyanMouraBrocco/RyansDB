#include "DatabaseFileReader.hpp"

DatabaseFileReader::DatabaseFileReader(std::ifstream &fileReader) : r_fileReader(fileReader)
{
    p_databaseDefintion = std::make_shared<DatabaseDefinition>();
}

DatabaseFileReader *DatabaseFileReader::LoadHeader()
{
    DatabaseHeaderFileReader databaseFileHeader(r_fileReader);
    auto header = databaseFileHeader.LoadAll()->Extract();
    p_databaseDefintion->SetHeader(header);
    return this;
}

DatabaseFileReader *DatabaseFileReader::LoadMappingPage()
{
    MappingFileReader mappingFileReader(r_fileReader);
    auto mappingFile = mappingFileReader.LoadAll()->Extract();
    p_databaseDefintion->SetTableMappingPage(mappingFile);
    return this;
}

DatabaseFileReader *DatabaseFileReader::LoadPageFreeSpace()
{
    PageFreeSpaceFileReader pageFreeSpaceFileReader(r_fileReader);
    auto pageFreeSpacePage = pageFreeSpaceFileReader.LoadAll()->Extract();
    p_databaseDefintion->SetPageFreeSpace(pageFreeSpacePage);
    return this;
}

DatabaseFileReader *DatabaseFileReader::LoadAll()
{
    LoadHeader();
    LoadMappingPage();
    LoadPageFreeSpace();
    return this;
}

DatabaseDefinition DatabaseFileReader::Extract()
{
    return *p_databaseDefintion;
}