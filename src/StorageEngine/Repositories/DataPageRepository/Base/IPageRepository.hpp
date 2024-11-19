#pragma once

#include "../../../../Error/error.hpp"
#include "../../../Contracts/Commands/InsertCommand.hpp"
#include "optional"
#include "variant"
#include "vector"

class IPageRepository
{
private:
public:
    virtual std::variant<int, Error> GetPageLength(int pageOffSet) = 0;
    virtual std::variant<std::vector<int>, Error> CreateNewBlockOfPages(int tableId) = 0;
    virtual std::optional<Error> InsertInPage(int pageOffSet, InsertCommand &command) = 0;
};