#pragma once

#include <memory>
#include <string>

#include "../../Contracts/Request/RequestProtocol.hpp"

class RequestProtocolBuilder
{
private:
    std::shared_ptr<RequestProtocol> m_protocol;
    std::string m_headerNameCache;

public:
    void AddVersion(std::string protocol);
    void AddCommandType(RequestProtocolCommandType commandType);
    void AddHeaderName(std::string headerName);
    void AddHeaderValue(std::string headerValue);
    void AddMessage(std::string message);

    RequestProtocol Build() const;
};