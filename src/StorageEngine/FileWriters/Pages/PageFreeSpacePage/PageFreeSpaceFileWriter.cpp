#include "PageFreeSpaceFileWriter.hpp"

PageFreeSpaceFileWriter::PageFreeSpaceFileWriter(std::ofstream &fileWriter) : r_fileWriter(fileWriter)
{
}

PageFreeSpaceFileWriter::PageFreeSpaceFileWriter(std::ofstream &fileWriter, int pageOffSet) : r_fileWriter(fileWriter), m_currentPageOffSet(pageOffSet)
{
}

void PageFreeSpaceFileWriter::WriteHeader(PageFreeSpacePageHeader &header)
{
    r_fileWriter.write(reinterpret_cast<char *>(header.GetNextPageOffSetRef()), sizeof(int));
    r_fileWriter.write(reinterpret_cast<char *>(header.GetPreviousPageOffsetRef()), sizeof(int));
}

void PageFreeSpaceFileWriter::SetHeader(PageFreeSpacePageHeader &header)
{
    r_fileWriter.seekp(m_currentPageOffSet, std::ios::beg);
    WriteHeader(header);
}

void PageFreeSpaceFileWriter::SetAll(PageFreeSpacePage &pageFreeSpace)
{
    r_fileWriter.seekp(m_currentPageOffSet, std::ios::beg);

    auto header = pageFreeSpace.GetHeader();
    WriteHeader(header);

    for (int i = 0; i < pageFreeSpace.GetFreePageSize(); i++)
    {
        r_fileWriter.write(reinterpret_cast<char *>(pageFreeSpace.GetFreePageRef(i)), sizeof(unsigned char));
    }

    if (!pageFreeSpace.IsFull())
    {
        r_fileWriter.seekp(m_currentPageOffSet + 8'000 - 1, std::ios::beg);
        r_fileWriter.write("\0", 1);
    }
}