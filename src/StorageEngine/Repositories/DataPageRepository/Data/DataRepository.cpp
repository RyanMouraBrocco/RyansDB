#include "DataRepository.hpp"

std::variant<int, Error> DataRepository::GetPageLength(int pageOffSet)
{
}

std::variant<std::vector<int>, Error> DataRepository::CreateNewBlockOfPages(int tableId)
{
}

std::optional<Error> DataRepository::InsertInPage(int pageOffSet, InsertCommand &command)
{
    std::fstream fileWriter("", std::ios::binary | std::ios::in | std::ios::out);

    // fileWriter.seekp(pageOffSet, std::ios::beg);
    // fileWriter.write(reinterpret_cast<char *>(), sizeof(int));

    // fileWriter.close();

    return std::nullopt;
}
