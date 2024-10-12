#include "TableProcessor.hpp"

std::optional<Error> TableProcessor::CreateTableExecution(std::string databaseName, std::shared_ptr<ParserTreeNode> queryTree)
{
    return p_createTableProcessor->CreateTableExecution(databaseName, queryTree);
}

std::shared_ptr<TableProcessor> TableProcessor::p_singleton = nullptr;
std::mutex TableProcessor::m_mutex;

std::shared_ptr<TableProcessor> TableProcessor::GetInstance()
{
    if (p_singleton == nullptr)
    {
        m_mutex.lock();
        if (p_singleton == nullptr)
        {
            p_singleton = std::make_shared<TableProcessor>();
        }
        m_mutex.unlock();
    }

    return p_singleton;
}