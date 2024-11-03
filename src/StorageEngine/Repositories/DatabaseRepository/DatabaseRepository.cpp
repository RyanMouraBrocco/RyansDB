#include "DatabaseRepository.hpp"

DatabaseRepository::DatabaseRepository()
{
    if (!std::filesystem::exists(m_databasePath))
        std::filesystem::create_directories(m_databasePath);
}

std::variant<DatabaseDefinition, Error> DatabaseRepository::GetDatabaseDefinition(std::string databaseName)
{
    std::fstream file(m_databasePath + "/" + databaseName + m_databaseExtension, std::ios::binary | std::ios::in);

    if (!file.is_open())
        return Error(ErrorType::Unexpected, "Error to fetch databasefile");

    DatabaseFileReader databaseFileReader(file);
    databaseFileReader.LoadAll();
    DatabaseDefinition database = databaseFileReader.Extract();

    file.close();

    return database;
}

std::optional<Error> DatabaseRepository::CreateDatabaseFile(DatabaseDefinition databaseDef)
{
    DatabaseHeader databaseHeader = databaseDef.GetHeader();
    std::fstream fileWriter(m_databasePath + "/" + databaseHeader.GetDatabaseNameRef() + m_databaseExtension, std::ios::binary | std::ios::out);

    if (!fileWriter.is_open())
        return Error(ErrorType::Unexpected, "Error to generate databasefile");

    DatabaseFileWriter databaseWriter(fileWriter);
    databaseWriter.SetAll(databaseDef);

    fileWriter.close();

    return std::nullopt;
}

bool DatabaseRepository::ExistsDatabase(std::string name)
{
    std::ifstream fileReader(m_databasePath + "/" + name + m_databaseExtension);
    return fileReader.good();
}

bool DatabaseRepository::ExistsTableInDatabase(std::string databaseName, std::string tableName)
{
    return false;
}

std::optional<Error> DatabaseRepository::DropDatabaseFile(std::string name)
{
    try
    {
        std::filesystem::remove(m_databasePath + "/" + name + m_databaseExtension);
        return std::nullopt;
    }
    catch (const std::filesystem::filesystem_error &err)
    {
        return Error(ErrorType::Unexpected, "Error to remove database file");
    }
}

std::optional<Error> DatabaseRepository::CreateTableInDatabaseFile(std::string databaseName, TableMappingPage tableMappingPage, std::shared_ptr<DataPage> dataPageBlock)
{
    auto databaseDefinitionResult = GetDatabaseDefinition(databaseName);
    if (std::holds_alternative<Error>(databaseDefinitionResult))
        return std::get<Error>(databaseDefinitionResult);

    auto databaseDefinition = std::get<DatabaseDefinition>(databaseDefinitionResult);

    std::fstream file(m_databasePath + "/" + databaseName + m_databaseExtension, std::ios::binary | std::ios::in | std::ios::out);

    if (!file.is_open())
        return Error(ErrorType::Unexpected, "Error to fetch databasefile");

    file.seekp(0, std::ios::end);
    int tableMappingStartPosition = (int)file.tellp();

    AddTableMappingPage(file, tableMappingPage, tableMappingStartPosition);

    for (int i = 0; i < 8; i++)
    {
        int nextPagePosition = (int)file.tellp();
        auto dataPage = dataPageBlock.get()[i];
        DataFileWriter dataFileWriter(file, nextPagePosition);
        dataFileWriter.SetAll(dataPage);
    }

    AddTableInMapping(file, databaseDefinition, tableMappingPage.GetHeader().GetTableId(), tableMappingStartPosition);
    AddPageFreeSpaceForANewTable(file, databaseDefinition);

    file.close();

    return std::nullopt;
}

void DatabaseRepository::AddTableInMapping(std::fstream &file, DatabaseDefinition &databaseDefinition, int tableId, int tableOffSet)
{
    MappingPage mapping = databaseDefinition.GetTableMappingPage();
    bool isRoot = true;
    int mappingPageOffSet = 0;
    int currentPageOffSet = 96;
    while (mapping.IsFull())
    {
        isRoot = false;
        int nextPageOffSet = mapping.GetHeader().GetNextPageOffSet();
        if (nextPageOffSet <= -1)
        {
            MappingPage newMappingPage;

            auto header = MappingPageHeader();
            header.SetPreviousPageOffSet(currentPageOffSet);
            newMappingPage.SetHeader(header);

            file.seekp(0, std::ios::end);
            int nextFreePageOffSet = (int)file.tellp();
            MappingFileWriter mappinFileWriter(file, nextFreePageOffSet);
            mappinFileWriter.SetAll(newMappingPage);

            auto currentHeader = mapping.GetHeader();
            currentHeader.SetNextPageOffSet(nextFreePageOffSet);
            mapping.SetHeader(currentHeader);
            MappingFileWriter currentMappinFileWriter(file, currentPageOffSet);
            currentMappinFileWriter.SetAll(mapping);

            mapping = newMappingPage;
            mappingPageOffSet = nextFreePageOffSet;
        }
        else
        {
            mappingPageOffSet = nextPageOffSet;
            MappingFileReader mappingFileReader(file, nextPageOffSet);
            mapping = mappingFileReader.LoadAll()->Extract();
        }

        currentPageOffSet = nextPageOffSet;
    }

    mapping.AddTableId(tableId);
    mapping.AddTableOffSet(tableOffSet);
    if (isRoot)
    {
        MappingFileWriter mappinFileWriter(file);
        mappinFileWriter.SetAll(mapping);
    }
    else
    {
        MappingFileWriter mappinFileWriter(file, mappingPageOffSet);
        mappinFileWriter.SetAll(mapping);
    }
}

void DatabaseRepository::AddPageFreeSpaceForANewTable(std::fstream &file, DatabaseDefinition &databaseDefinition)
{
    auto pageFreeSpace = databaseDefinition.GetPageFreeSpace();
    bool isRoot = true;
    int pageFreeSpacePageOffSet = 0;
    int currentPageOffSet = 96 + 8'000;
    while (pageFreeSpace.IsFull())
    {
        isRoot = false;
        int nextPageOffSet = pageFreeSpace.GetHeader().GetNextPageOffSet();
        if (nextPageOffSet <= -1)
        {
            PageFreeSpacePage newFreeSpacePage;

            auto header = PageFreeSpacePageHeader();
            header.SetPreviousPageOffSet(currentPageOffSet);
            newFreeSpacePage.SetHeader(header);

            file.seekp(0, std::ios::end);
            int nextFreePageOffSet = (int)file.tellp();
            PageFreeSpaceFileWriter pageFreeSpaceFileWriter(file, nextFreePageOffSet);
            pageFreeSpaceFileWriter.SetAll(newFreeSpacePage);

            auto currentHeader = pageFreeSpace.GetHeader();
            currentHeader.SetNextPageOffSet(nextFreePageOffSet);
            pageFreeSpace.SetHeader(currentHeader);
            PageFreeSpaceFileWriter currentPageFreeSpaceFileWriter(file, currentPageOffSet);
            currentPageFreeSpaceFileWriter.SetAll(pageFreeSpace);

            pageFreeSpace = newFreeSpacePage;
            pageFreeSpacePageOffSet = nextFreePageOffSet;
            nextPageOffSet = nextFreePageOffSet;
        }
        else
        {
            pageFreeSpacePageOffSet = nextPageOffSet;
            PageFreeSpaceFileReader pageFreeSpaceFileReader(file, nextPageOffSet);
            pageFreeSpace = pageFreeSpaceFileReader.LoadAll()->Extract();
        }
        currentPageOffSet = nextPageOffSet;
    }

    pageFreeSpace.AddFreePageValue(0); // tableMappingPage
    for (int i = 0; i < 8; i++)
    {
        if (pageFreeSpace.IsFull())
        {
            isRoot = false;
            PageFreeSpacePage newFreeSpacePage;

            auto header = PageFreeSpacePageHeader();
            header.SetPreviousPageOffSet(currentPageOffSet);
            newFreeSpacePage.SetHeader(header);

            file.seekp(0, std::ios::end);
            int nextFreePageOffSet = (int)file.tellp();
            PageFreeSpaceFileWriter pageFreeSpaceFileWriter(file, nextFreePageOffSet);
            pageFreeSpaceFileWriter.SetAll(newFreeSpacePage);

            auto currentHeader = pageFreeSpace.GetHeader();
            currentHeader.SetNextPageOffSet(nextFreePageOffSet);
            pageFreeSpace.SetHeader(currentHeader);
            PageFreeSpaceFileWriter currentPageFreeSpaceFileWriter(file, currentPageOffSet);
            currentPageFreeSpaceFileWriter.SetAll(pageFreeSpace);

            pageFreeSpace = newFreeSpacePage;
            pageFreeSpacePageOffSet = nextFreePageOffSet;
        }
        pageFreeSpace.AddFreePageValue(0);
    }

    if (isRoot)
    {
        PageFreeSpaceFileWriter pageFreeSpaceFileWriter(file);
        pageFreeSpaceFileWriter.SetAll(pageFreeSpace);
    }
    else
    {
        PageFreeSpaceFileWriter pageFreeSpaceFileWriter(file, pageFreeSpacePageOffSet);
        pageFreeSpaceFileWriter.SetAll(pageFreeSpace);
    }
}

void DatabaseRepository::AddTableMappingPage(std::fstream &file, TableMappingPage tableMappingPage, int tableMappingStartPosition)
{
    auto header = tableMappingPage.GetHeader();

    for (int i = 0; i < 8; i++)
        tableMappingPage.SetTableBlockMap(header.GetLastMappedPageIndex() + i, true);

    header.SetLastMappedPageIndex(header.GetLastMappedPageIndex() + 8);
    header.SetStartPageOffSet(tableMappingStartPosition + 8'000);

    tableMappingPage.SetHeader(header);

    TableMappingFileWriter tableMappinFireWriter(file, tableMappingStartPosition);
    tableMappinFireWriter.SetAll(tableMappingPage);
}