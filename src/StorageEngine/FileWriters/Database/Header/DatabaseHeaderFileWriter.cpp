#include "DatabaseHeaderFileWriter.hpp"

DatabaseHeaderFileWriter::DatabaseHeaderFileWriter(std::ofstream &fileWriter) : r_fileWriter(fileWriter)
{
}

void DatabaseHeaderFileWriter::SetId(int *id)
{
    r_fileWriter.seekp(0, std::ios::beg);
    r_fileWriter.write(reinterpret_cast<char *>(id), sizeof(int));
}

void DatabaseHeaderFileWriter::SetDatabaseName(char *name)
{
    r_fileWriter.seekp(sizeof(int), std::ios::beg);
    r_fileWriter.write(name, 50 * sizeof(char));
}

void DatabaseHeaderFileWriter::SetFileLength(int *fileLength)
{
    r_fileWriter.seekp(sizeof(int) + (50 * sizeof(char)), std::ios::beg);
    r_fileWriter.write(reinterpret_cast<char *>(fileLength), sizeof(int));
}

void DatabaseHeaderFileWriter::SetAll(DatabaseHeader &header)
{
    r_fileWriter.seekp(0, std::ios::beg);
    r_fileWriter.write(reinterpret_cast<char *>(header.GetIdRef()), sizeof(int));
    r_fileWriter.write(header.GetDatabaseNameRef(), 50 * sizeof(char));
    r_fileWriter.write(reinterpret_cast<char *>(header.GetFileLengthRef()), sizeof(int));

    r_fileWriter.seekp(95, std::ios::beg);
    r_fileWriter.write("\0", 1);
}
