#pragma once

#include "../Base/IPageRepository.hpp"

class DataRepository : public IPageRepository
{
private:
public:
    virtual std::optional<Error> InsertInPage(int pageOffSet) override;
};