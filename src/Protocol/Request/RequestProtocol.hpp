#pragma once

#include "../ProtocolTemplate.hpp"
#include <unordered_set>

enum class RequestProtocolCommandType
{
    RawQuery,
    StoredProcedure
};

static std::unordered_set<std::string> m_headersWhiteList{
    "Charset",
    "Content-Type",
    "Content-Encoding"};

class RequestProtocol : public ProtocolTemplate
{
private:
    RequestProtocolCommandType m_commandType;

public:
    RequestProtocol();
    const void AddHeader(std::string &key, std::string &value) override;
    const RequestProtocolCommandType GetCommandType() const;
    const void SetCommandType(RequestProtocolCommandType commandType);
};