#pragma once

#include <string>
#include "../Protocol/Serialization/RequestProtocolDeserializer.hpp"

class RequestOrchestrator
{
public:
    RequestOrchestrator();
    void Consume(std::string requestMessage);
};