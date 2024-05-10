#include "ResponseProtocol.hpp"

ResponseProtocol::ResponseProtocol()
{
}

const void ResponseProtocol::AddHeader(std::string &key, std::string &value)
{
    if (m_headersWhiteList.find(value) != m_headersWhiteList.end())
        ProtocolTemplate::AddHeader(key, value);
}

const ResponseProtocolStatusCode ResponseProtocol::GetStatusCode() const
{
    return m_statusCode;
}

const void ResponseProtocol::SetStatusCode(ResponseProtocolStatusCode statusCode)
{
    m_statusCode = statusCode;
}

const std::string ResponseProtocol::GetErrorDescription() const
{
    return m_errorDescription;
}

const void ResponseProtocol::SetErrorDescription(std::string &errorDescription)
{
    m_errorDescription = errorDescription;
}