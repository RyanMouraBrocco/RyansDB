#pragma once

#include <string>
#include "../Protocol/Serialization/RequestProtocolDeserializer.hpp"
#include "../Compiler/Compiler.hpp"
#include "../ExecutePlan/ExecuteEngine.hpp"

class RequestOrchestrator
{
public:
    RequestOrchestrator();
    void Consume(std::string requestMessage);
};