#include "DatabaseFileWriter.hpp"

DatabaseFileWriter::DatabaseFileWriter(std::ofstream &fileWriter) : r_fileWriter(fileWriter)
{
}

void DatabaseFileWriter::SetHeader(DatabaseHeader &header)
{
    DatabaseHeaderFileWriter databaseHeaderWriter(r_fileWriter);
    databaseHeaderWriter.SetAll(header);
}

void DatabaseFileWriter::SetMapping(MappingPage &mapping)
{
    MappingFileWriter mappingPageWriter(r_fileWriter);
    mappingPageWriter.SetAll(mapping);
}

void DatabaseFileWriter::SetPageFreeSpace(PageFreeSpacePage &pageFreeSpace)
{
    PageFreeSpaceFileWriter pageFreeSpaceFileWriter(r_fileWriter);
    pageFreeSpaceFileWriter.SetAll(pageFreeSpace);
}

void DatabaseFileWriter::SetAll(DatabaseDefinition &databaseDefinition)
{
    auto header = databaseDefinition.GetHeader();
    auto mappingPage = databaseDefinition.GetTableMappingPage();
    auto pageFreeSpace = databaseDefinition.GetPageFreeSpace();

    SetHeader(header);
    SetMapping(mappingPage);
    SetPageFreeSpace(pageFreeSpace);
}