#pragma once

#include "../Base/IPageRepository.hpp"
#include "../../../FileReaders/Pages/DataFileReader.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>

class DataRepository : public IPageRepository
{
private:
public:
    virtual std::variant<int, Error> GetPageLength(int pageOffSet) override;
    virtual std::variant<std::vector<int>, Error> CreateNewBlockOfPages(int tableId) override;
    virtual std::optional<Error> InsertInPage(int pageOffSet, InsertCommand &command) override;
};