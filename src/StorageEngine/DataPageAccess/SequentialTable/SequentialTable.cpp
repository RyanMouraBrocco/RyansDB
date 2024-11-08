#include "SequentialTable.hpp"

SequentialTable::SequentialTable(std::vector<int> pagesOffSet) : m_pagesOffSet(pagesOffSet)
{
}

std::optional<Error> SequentialTable::InsertOne(InsertCommand command)
{
    auto lastPageOffSet = m_pagesOffSet.back();
    
}