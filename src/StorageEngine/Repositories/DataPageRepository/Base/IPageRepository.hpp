#pragma once

#include "../../../../Error/error.hpp"
#include "optional"

class IPageRepository
{
private:
public:
    virtual std::optional<Error> InsertInPage(int pageOffSet) = 0;
};