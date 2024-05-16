#pragma once

#include "../ProtocolTemplate.hpp"
#include <unordered_set>

enum class ResponseProtocolStatusCode
{
    Success = 200
};

static std::unordered_set<std::string> m_headersWhiteList{
    "Charset",
    "Content-Type",
    "Content-Encoding"};

class ResponseProtocol : public ProtocolTemplate
{
private:
    ResponseProtocolStatusCode m_statusCode;
    std::string m_errorDescription;

public:
    ResponseProtocol();
    const void AddHeader(std::string &key, std::string &value) override;
    const ResponseProtocolStatusCode GetStatusCode() const;
    const void SetStatusCode(ResponseProtocolStatusCode statusCode);
    const std::string GetErrorDescription() const;
    const void SetErrorDescription(std::string &errorDescription);
};