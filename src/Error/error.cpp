#include "error.hpp"

Error::Error(ErrorType type, std::string description) : m_type(type), m_description(description)
{
}