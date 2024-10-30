#include "DatabaseRepository.hpp"

DatabaseRepository::DatabaseRepository()
{
    if (!std::filesystem::exists(m_databasePath))
        std::filesystem::create_directories(m_databasePath);
}

std::variant<DatabaseDefinition, Error> DatabaseRepository::GetDatabaseDefinition(std::string databaseName)
{
    std::ifstream file(m_databasePath + "/" + databaseName + m_databaseExtension, std::ios::binary);

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
    int tableMappingStartPosition = (int)file.tellp() + 1;

    auto header = tableMappingPage.GetHeader();

    for (int i = 0; i < 8; i++)
        tableMappingPage.SetTableBlockMap(header.GetLastMappedPageIndex() + i, true);

    header.SetLastMappedPageIndex(header.GetLastMappedPageIndex() + 8);
    header.SetStartPageOffSet(tableMappingStartPosition + 8'000);

    tableMappingPage.SetHeader(header);

    TableMappingFileWriter tableMappinFireWriter(file, tableMappingStartPosition);
    tableMappinFireWriter.SetAll(tableMappingPage);

    for (int i = 0; i < 8; i++)
    {
        int nextPagePosition = (int)file.tellp();
        auto dataPage = dataPageBlock.get()[i];
        DataFileWriter dataFileWriter(file, nextPagePosition);
        dataFileWriter.SetAll(dataPage);
    }

    auto mapping = databaseDefinition.GetTableMappingPage();
    mapping.AddTableId(tableMappingPage.GetHeader().GetTableId());
    mapping.AddTableOffSet(tableMappingStartPosition);
    MappingFileWriter mappinFileWriter(file);
    mappinFileWriter.SetAll(mapping);

    auto pageFreeSpace = databaseDefinition.GetPageFreeSpace();
    pageFreeSpace.AddFreePageValue(0); // tableMappingPage
    for (int i = 0; i < 8; i++)
        pageFreeSpace.AddFreePageValue(0);

    PageFreeSpaceFileWriter pageFreeSpaceFileWriter(file);
    pageFreeSpaceFileWriter.SetAll(pageFreeSpace);

    file.close();

    return std::nullopt;
}