#pragma once

#include "./CreateTableProcessor/CreateTableProcessor.hpp"

class TableProcessor
{
private:
    static std::shared_ptr<TableProcessor> p_singleton;
    static std::mutex m_mutex;

    std::shared_ptr<CreateTableProcessor> p_createTableProcessor = CreateTableProcessor::GetInstance();

public:
    static std::shared_ptr<TableProcessor> GetInstance();
    std::optional<Error> CreateTableExecution(std::string databaseName, std::shared_ptr<ParserTreeNode> queryTree);
};