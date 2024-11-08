#pragma once

#include "../Base/IPageRepository.hpp"
#include "memory"

class DataPageRepositoryFactory
{
private:
public:
    std::shared_ptr<IPageRepository> CreateDataPageRepository();
};