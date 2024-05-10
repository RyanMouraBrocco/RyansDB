#include "RequestProtocol.hpp"

RequestProtocol::RequestProtocol()
{
}

const RequestProtocolCommandType RequestProtocol::GetCommandType() const
{
    return m_commandType;
}

const void RequestProtocol::SetCommandType(RequestProtocolCommandType commandType)
{
    m_commandType = commandType;
}

const void RequestProtocol::AddHeader(std::string &key, std::string &value)
{
    if (m_headersWhiteList.find(value) != m_headersWhiteList.end())
        ProtocolTemplate::AddHeader(key, value);
}
