#pragma once

#include "string"
#include "vector"
#include "memory"
#include "../Base/ITableAccess.hpp"

class TableAccessFactory
{
public:
    std::shared_ptr<ITableAccess> CreateSequentialTable();
    std::shared_ptr<ITableAccess> CreateClusteredTable();
};