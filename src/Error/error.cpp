#include "error.hpp"

Error::Error(std::string description, ErrorType type) : m_description(description), m_type(type)
{
}