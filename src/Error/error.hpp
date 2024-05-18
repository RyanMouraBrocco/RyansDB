#pragma once
#include <string>

enum class ErrorType
{
    ProtocolFormat = 400,
    Unexpected = 500
};

class Error
{
    std::string m_description;
    ErrorType m_type;

public:
    Error(ErrorType type, std::string description);
};