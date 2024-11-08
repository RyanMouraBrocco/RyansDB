#pragma once

#include "../Base/ITableAccess.hpp"

class SequentialTable : public ITableAccess
{
private:
    std::vector<int> m_pagesOffSet;

public:
    SequentialTable(std::vector<int> pagesOffSet);
    std::optional<Error> InsertOne(InsertCommand command) override;
};