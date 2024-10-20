#include "DatabaseHeaderFileReader.hpp"

DatabaseHeaderFileReader::DatabaseHeaderFileReader(std::ifstream &fileReader) : r_fileReader(fileReader)
{
    p_databaseHeader = std::make_shared<DatabaseHeader>();
}

void DatabaseHeaderFileReader::FetchId()
{
    r_fileReader.read(reinterpret_cast<char *>(p_databaseHeader->GetIdRef()), sizeof(int));
}

void DatabaseHeaderFileReader::FetchDatabaseName()
{
    r_fileReader.read(p_databaseHeader->GetDatabaseNameRef(), 50 * sizeof(char));
}

void DatabaseHeaderFileReader::FetchFileLength()
{
    r_fileReader.read(reinterpret_cast<char *>(p_databaseHeader->GetFileLengthRef()), sizeof(int));
}

DatabaseHeaderFileReader *DatabaseHeaderFileReader::LoadId()
{
    r_fileReader.seekg(0, std::ios::beg);
    FetchId();
    return this;
}

DatabaseHeaderFileReader *DatabaseHeaderFileReader::LoadDatabaseName()
{
    r_fileReader.seekg(sizeof(int), std::ios::beg);
    FetchDatabaseName();
    return this;
}

DatabaseHeaderFileReader *DatabaseHeaderFileReader::LoadFileLength()
{
    r_fileReader.seekg(sizeof(int) + (50 * sizeof(char)), std::ios::beg);
    FetchFileLength();
    return this;
}

DatabaseHeaderFileReader *DatabaseHeaderFileReader::LoadAll()
{
    r_fileReader.seekg(0, std::ios::beg);
    FetchId();
    FetchDatabaseName();
    FetchFileLength();
    return this;
}

DatabaseHeader DatabaseHeaderFileReader::Extract()
{
    return *p_databaseHeader;
}