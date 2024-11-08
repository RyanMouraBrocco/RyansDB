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
    std::fstream fileWriter(m_databasePath + "/" + databaseDef.GetHeaderRef().GetDatabaseNamePointer() + m_databaseExtension, std::ios::binary | std::ios::out);

    if (!fileWriter.is_open())
        return Error(ErrorType::Unexpected, "Error to generate databasefile");

    DatabaseFileWriter databaseWriter(fileWriter);
    databaseWriter.SetAll(databaseDef);

    TableMappingPage tableMappingPage(-100);
    std::shared_ptr<DataPage> dataPageBlock(new DataPage[8], std::default_delete<DataPage[]>());
    for (int i = 0; i < 8; i++)
    {
        dataPageBlock.get()[i].GetHeaderRef().SetPageId(i + 1);
        dataPageBlock.get()[i].GetHeaderRef().SetPageLength(0);
        dataPageBlock.get()[i].GetHeaderRef().SetTableId(-100);
    }

    CreateTable(fileWriter, databaseDef, tableMappingPage, dataPageBlock);

    fileWriter.close();

    return std::nullopt;
}

bool DatabaseRepository::ExistsDatabase(std::string name)
{
    std::ifstream fileReader(m_databasePath + "/" + name + m_databaseExtension);
    return fileReader.good();
}

std::variant<bool, Error> DatabaseRepository::ExistsTableInDatabase(std::string databaseName, int tableId)
{
    std::fstream fileReader(m_databasePath + "/" + databaseName + m_databaseExtension, std::ios::binary | std::ios::in);

    if (!fileReader.is_open())
        return Error(ErrorType::Unexpected, "Error to generate databasefile");

    DatabaseFileReader databaseFileReader(fileReader);
    auto databaseFileDef = databaseFileReader.LoadMappingPage()->Extract();
    MappingPage &mappingPage = databaseFileDef.GetTableMappingPageRef();
    bool exists = false;
    while (!exists)
    {
        for (int i = 0; i < mappingPage.GetTablesMapSize(); i++)
        {
            if (mappingPage.GetTableIdByIndex(i) == tableId)
            {
                exists = true;
            }
        }

        auto nextPage = mappingPage.GetHeaderRef().GetNextPageOffSet();
        if (!mappingPage.IsFull() || nextPage == -1)
            break;

        MappingFileReader mappingPageFileReader(fileReader, nextPage);
        mappingPage = mappingPageFileReader.LoadAll()->Extract();
    }

    fileReader.close();

    return exists;
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

    CreateTable(file, databaseDefinition, tableMappingPage, dataPageBlock);

    file.close();

    return std::nullopt;
}

void DatabaseRepository::CreateTable(std::fstream &file, DatabaseDefinition &databaseDefinition, TableMappingPage &tableMappingPage, std::shared_ptr<DataPage> dataPageBlock)
{
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

    AddTableInMapping(file, databaseDefinition, tableMappingPage.GetHeaderRef().GetTableId(), tableMappingStartPosition);
    AddPageFreeSpaceForANewTable(file, databaseDefinition);
}

void DatabaseRepository::AddTableInMapping(std::fstream &file, DatabaseDefinition &databaseDefinition, int tableId, int tableOffSet)
{
    auto [mappingPageOffSet, mapping] = CreateIfNotExistsLastMappingPage(file, databaseDefinition);
    mapping.AddTableId(tableId);
    mapping.AddTableOffSet(tableOffSet);
    MappingFileWriter mappinFileWriter(file, mappingPageOffSet);
    mappinFileWriter.SetAll(mapping);
}

std::tuple<int, MappingPage> DatabaseRepository::CreateIfNotExistsLastMappingPage(std::fstream &file, DatabaseDefinition &databaseDefinition)
{
    MappingPage mapping = databaseDefinition.GetTableMappingPage();
    int currentPageOffSet = 96;
    while (mapping.IsFull())
    {
        int nextPageOffSet = mapping.GetHeaderRef().GetNextPageOffSet();
        if (nextPageOffSet > -1)
        {
            MappingFileReader mappingFileReader(file, nextPageOffSet);
            mapping = mappingFileReader.LoadAll()->Extract();
            currentPageOffSet = nextPageOffSet;
        }
        else
        {
            auto [pageFreeSpacePageOffSet, pageFreeSpace] = CreateIfNotExistsLastPageFreeSpacePage(file, databaseDefinition);
            pageFreeSpace.AddFreePageValue(0);
            PageFreeSpaceFileWriter pageFreeSpaceFileWriter(file, pageFreeSpacePageOffSet);
            pageFreeSpaceFileWriter.SetAll(pageFreeSpace);

            MappingPage newMappingPage;
            newMappingPage.GetHeaderRef().SetPreviousPageOffSet(currentPageOffSet);

            file.seekp(0, std::ios::end);
            int nextFreePageOffSet = (int)file.tellp();

            MappingFileWriter mappinFileWriter(file, nextFreePageOffSet);
            mappinFileWriter.SetAll(newMappingPage);

            mapping.GetHeaderRef().SetNextPageOffSet(nextFreePageOffSet);
            MappingFileWriter currentMappinFileWriter(file, currentPageOffSet);
            currentMappinFileWriter.SetAll(mapping);

            mapping = newMappingPage;
            currentPageOffSet = nextFreePageOffSet;
        }
    }

    return std::tuple<int, MappingPage>(currentPageOffSet, mapping);
}

void DatabaseRepository::AddPageFreeSpaceForANewTable(std::fstream &file, DatabaseDefinition &databaseDefinition)
{
    auto [pageFreeSpacePageOffSet, pageFreeSpace] = CreateIfNotExistsLastPageFreeSpacePage(file, databaseDefinition);
    pageFreeSpace.AddFreePageValue(0); // tableMappingPage

    for (int i = 0; i < 8; i++)
    {
        if (pageFreeSpace.IsFull())
        {
            PageFreeSpaceFileWriter pageFreeSpaceFileWriter(file, pageFreeSpacePageOffSet);
            pageFreeSpaceFileWriter.SetAll(pageFreeSpace);
            std::tie(pageFreeSpacePageOffSet, pageFreeSpace) = CreateInBackNewPageFreeSpace(file, pageFreeSpace, pageFreeSpacePageOffSet);
        }

        pageFreeSpace.AddFreePageValue(0);
    }

    PageFreeSpaceFileWriter pageFreeSpaceFileWriter(file, pageFreeSpacePageOffSet);
    pageFreeSpaceFileWriter.SetAll(pageFreeSpace);
}

std::tuple<int, PageFreeSpacePage> DatabaseRepository::CreateIfNotExistsLastPageFreeSpacePage(std::fstream &file, DatabaseDefinition &databaseDefinition)
{
    auto pageFreeSpace = databaseDefinition.GetPageFreeSpace();
    int currentPageOffSet = 96 + 8'000;
    while (pageFreeSpace.IsFull())
    {
        int nextPageOffSet = pageFreeSpace.GetHeaderRef().GetNextPageOffSet();
        if (nextPageOffSet > -1)
        {
            PageFreeSpaceFileReader pageFreeSpaceFileReader(file, nextPageOffSet);
            pageFreeSpace = pageFreeSpaceFileReader.LoadAll()->Extract();
            currentPageOffSet = nextPageOffSet;
        }
        else
        {
            auto [nextFreePageOffSet, newFreeSpacePage] = CreateInBackNewPageFreeSpace(file, pageFreeSpace, currentPageOffSet);
            pageFreeSpace = newFreeSpacePage;
            currentPageOffSet = nextFreePageOffSet;
        }
    }

    return std::tuple<int, PageFreeSpacePage>(currentPageOffSet, pageFreeSpace);
}

std::tuple<int, PageFreeSpacePage> DatabaseRepository::CreateInBackNewPageFreeSpace(std::fstream &file, PageFreeSpacePage &currentPage, int currentPageOffSet)
{
    PageFreeSpacePage newFreeSpacePage;
    newFreeSpacePage.GetHeaderRef().SetPreviousPageOffSet(currentPageOffSet);

    file.seekp(0, std::ios::end);
    int nextFreePageOffSet = (int)file.tellp();
    PageFreeSpaceFileWriter pageFreeSpaceFileWriter(file, nextFreePageOffSet);
    pageFreeSpaceFileWriter.SetAll(newFreeSpacePage);

    currentPage.GetHeaderRef().SetNextPageOffSet(nextFreePageOffSet);
    PageFreeSpaceFileWriter currentPageFreeSpaceFileWriter(file, currentPageOffSet);
    currentPageFreeSpaceFileWriter.SetAll(currentPage);

    return std::tuple<int, PageFreeSpacePage>(nextFreePageOffSet, newFreeSpacePage);
}

void DatabaseRepository::AddTableMappingPage(std::fstream &file, TableMappingPage tableMappingPage, int tableMappingStartPosition)
{
    TableMappingPageHeader &header = tableMappingPage.GetHeaderRef();

    for (int i = 0; i < 8; i++)
        tableMappingPage.SetTableBlockMap(header.GetLastMappedPageIndex() + i, true);

    header.SetLastMappedPageIndex(header.GetLastMappedPageIndex() + 8);
    header.SetStartPageOffSet(tableMappingStartPosition + 8'000);

    TableMappingFileWriter tableMappinFireWriter(file, tableMappingStartPosition);
    tableMappinFireWriter.SetAll(tableMappingPage);
}