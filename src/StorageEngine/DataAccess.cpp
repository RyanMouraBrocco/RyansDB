#include "DataAccess.hpp"

int DataAccess::CalcIdFromName(std::string name)
{
    std::hash<std::string> hasher;
    int hasedValue = hasher(name);
    return hasedValue < 0 ? hasedValue * -1 : hasedValue;
}

std::optional<Error> DataAccess::CreateDatabaseFile(std::string name)
{
    if (m_databaseRepository.ExistsDatabase(name))
        return Error(ErrorType::Unexpected, "Database already exists");

    int databaseId = CalcIdFromName(name);

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

    int tableId = CalcIdFromName(tableName);

    auto existsResult = m_databaseRepository.ExistsTableInDatabase(databaseName, tableId);
    if (std::holds_alternative<Error>(existsResult) || std::get<bool>(existsResult))
        return Error(ErrorType::Unexpected, "Table already exists");

    auto databaseDefinitionResult = m_databaseRepository.GetDatabaseDefinition(databaseName);
    if (std::holds_alternative<Error>(databaseDefinitionResult))
        return std::get<Error>(databaseDefinitionResult);

    auto databaseDefinition = std::get<DatabaseDefinition>(databaseDefinitionResult);

    TableMappingPage tablePage(tableId);

    std::shared_ptr<DataPage> dataPageBlock(new DataPage[8], std::default_delete<DataPage[]>());
    for (int i = 0; i < 8; i++)
    {
        dataPageBlock.get()[i].GetHeaderRef().SetPageId(i + 1);
        dataPageBlock.get()[i].GetHeaderRef().SetPageLength(0);
        dataPageBlock.get()[i].GetHeaderRef().SetTableId(tableId);
    }

    auto tableCreationResult = m_databaseRepository.CreateTableInDatabaseFile(databaseDefinition, tablePage, dataPageBlock);
    if (tableCreationResult.has_value())
        return tableCreationResult;

    return std::nullopt;

    // auto tableAccess = new TableAccessFactory();
    // auto specialTableAccess = tableAccess->CreateSequentialTable();
    // return specialTableAccess->InsertOne(InsertCommand(-100, std::vector<InsertData>{
    //                                                              InsertData("coluna 1"),
    //                                                              InsertData("coluna 2"),
    //                                                          }));
}