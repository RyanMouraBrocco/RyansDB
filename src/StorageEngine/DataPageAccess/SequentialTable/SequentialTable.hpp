#pragma once

#include "../Base/ITableAccess.hpp"
#include "../../Repositories/DataPageRepository/Data/DataRepository.hpp"

class SequentialTable : public ITableAccess
{
private:
    int m_tableId;
    std::vector<int> m_pagesOffSet;
    DataRepository m_dataRepository;

public:
    SequentialTable(int tableId, std::vector<int> pagesOffSet);
    std::optional<Error> InsertOne(InsertCommand command) override;
};