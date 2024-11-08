#pragma once

#include "../Commands/InsertCommand.hpp"
#include "optional"
#include "../../../Error/error.hpp"

class ITableAccess
{
    virtual std::optional<Error> InsertOne(InsertCommand command) = 0;
};