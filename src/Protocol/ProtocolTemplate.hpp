#pragma once

#include <string>
#include <map>

class ProtocolTemplate
{
private:
    std::string m_version;
    std::map<std::string, std::string> m_headers;
    std::string m_message;

public:
    ProtocolTemplate(){};

    const std::string GetVersion() const
    {
        return m_version;
    }

    const std::map<std::string, std::string> GetHeaders() const
    {
        return m_headers;
    }

    const std::string GetMessage() const
    {
        return m_message;
    }

    const void SetVersion(std::string &version)
    {
        m_version = version;
    }

    const void SetMessage(std::string &message)
    {
        m_message = message;
    }

    virtual const void AddHeader(std::string &key, std::string &value)
    {
        if (m_headers.find(key) == m_headers.end())
            m_headers.insert({key, value});
    }
};