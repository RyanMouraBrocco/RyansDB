#pragma once

#include "memory"
#include "mutex"
#include "optional"
#include "../../../Error/error.hpp"
#include "../../../Compiler/SymbolTable/ParserTree/ParserTreeNode.hpp"
#include "../../../StorageEngine/DataAccess.hpp"

class DatabaseProcessor
{
private:
    static std::shared_ptr<DatabaseProcessor> p_singleton;
    static std::mutex m_mutex;

    DataAccess m_dataAccess;

public:
    static std::shared_ptr<DatabaseProcessor> GetInstance();
    std::optional<Error> CreateDatabaseExecution(std::shared_ptr<ParserTreeNode> queryTree);
    std::optional<Error> DropDatabaseExecution(std::shared_ptr<ParserTreeNode> queryTree);
};