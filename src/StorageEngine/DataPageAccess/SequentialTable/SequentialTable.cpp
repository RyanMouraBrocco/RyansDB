#include "SequentialTable.hpp"

SequentialTable::SequentialTable(int tableId, std::vector<int> pagesOffSet) : m_pagesOffSet(pagesOffSet), m_tableId(tableId)
{
}

std::optional<Error> SequentialTable::InsertOne(InsertCommand command)
{
    auto lastPageOffSet = m_pagesOffSet.back();
    auto pageLengthResult = m_dataRepository.GetPageLength(lastPageOffSet);
    if (std::holds_alternative<Error>(pageLengthResult))
        return std::get<Error>(pageLengthResult);

    if (std::get<int>(pageLengthResult) + command.GetAllDataLength() > 8'000)
    {
        auto newBlockResult = m_dataRepository.CreateNewBlockOfPages(m_tableId);
        if (std::holds_alternative<Error>(newBlockResult))
            return std::get<Error>(newBlockResult);

        auto newPages = std::get<std::vector<int>>(newBlockResult);
        for (int i = 0; i < newPages.size(); i++)
        {
            m_pagesOffSet.push_back(newPages[i]);
            if (i == newPages.size() - 1)
                lastPageOffSet = newPages[i];
        }
    }

    m_dataRepository.InsertInPage(lastPageOffSet, command);
}