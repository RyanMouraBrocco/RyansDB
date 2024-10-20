#include "DataAccess.hpp"

std::optional<Error> DataAccess::CreateDatabaseFile(std::string name)
{
    if (m_databaseRepository.ExistsDatabase(name))
        return Error(ErrorType::Unexpected, "Database already exists");

    int databaseId = 0;
    for (int i = 0; i < name.length(); i++)
    {
        databaseId += name[i];
    }

    DatabaseDefinition databaseDefinition(databaseId, name, 16'000 + 96);
    return m_databaseRepository.CreateDatabaseFile(databaseDefinition);
}

std::optional<Error> DataAccess::DropDatabaseFile(std::string name)
{
    if (!m_databaseRepository.ExistsDatabase(name))
        return Error(ErrorType::Unexpected, "Database does not exist");

    return m_databaseRepository.DropDatabaseFile(name);
}

std::optional<Error> DataAccess::CreateTableInDatabaseFile(std::string databaseName, std::string tableName, std::vector<TableColumnDefinition> columnsDefinitions)
{
    if (!m_databaseRepository.ExistsDatabase(databaseName))
        return Error(ErrorType::Unexpected, "Database does not exist");

    if (m_databaseRepository.ExistsTableInDatabase(databaseName, tableName))
        return Error(ErrorType::Unexpected, "Table already exists");

    int tableId = 0;
    for (int i = 0; i < tableName.length(); i++)
    {
        tableId += tableName[i];
    }

    TableMappingPage tablePage(tableId);

    std::shared_ptr<DataPage> dataPageBlock(new DataPage[8], std::default_delete<DataPage[]>());
    for (int i = 0; i < 8; i++)
    {
        dataPageBlock.get()[i].GetHeaderRef()->SetPageId(i + 1);
        dataPageBlock.get()[i].GetHeaderRef()->SetPageLength(0);
        dataPageBlock.get()[i].GetHeaderRef()->SetTableId(tableId);
    }

    return m_databaseRepository.CreateTableInDatabaseFile(databaseName, tablePage, dataPageBlock);
}