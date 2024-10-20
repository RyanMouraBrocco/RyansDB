#include "DatabaseFileReader.hpp"

DatabaseFileReader *DatabaseFileReader::LoadHeader()
{
    DatabaseHeaderFileReader databaseFileHeader(r_fileReader);
    databaseFileHeader.LoadAll();
    p_databaseDefintion->SetHeader(databaseFileHeader);
    return this;
}
