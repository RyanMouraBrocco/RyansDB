#pragma once
#include <string>

enum class ErrorType
{
    Unexpected
};

class Error
{
    std::string m_description;
    ErrorType m_type;

public:
    Error(std::string description, ErrorType type);
};