#include "RequestProtocolBuilder.hpp"

RequestProtocolBuilder::RequestProtocolBuilder()
{
    m_protocol = std::make_shared<RequestProtocol>();
}

void RequestProtocolBuilder::AddVersion(std::string protocol)
{
    m_protocol->SetVersion(protocol);
}

void RequestProtocolBuilder::AddCommandType(RequestProtocolCommandType commandType)
{
    m_protocol->SetCommandType(commandType);
}

void RequestProtocolBuilder::AddHeaderName(std::string headerName)
{
    m_headerNameCache = headerName;
}

void RequestProtocolBuilder::AddHeaderValue(std::string headerValue)
{
    if (m_headerNameCache.length() > 0)
    {
        m_protocol->AddHeader(m_headerNameCache, headerValue);
        m_headerNameCache = "";
    }
}

void RequestProtocolBuilder::AddMessage(std::string message)
{
    m_protocol->SetMessage(message);
}

RequestProtocol RequestProtocolBuilder::Build() const
{
    return *m_protocol;
}