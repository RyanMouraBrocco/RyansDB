#pragma once
#include <string>

enum class ErrorType
{
    // BEGIN Protocol Error
    ProtocolFormat = 400,
    // END Protocol Error

    Unexpected = 500,

    // BEGIN Compiler Error
    InvalidReservedStatement = 600,
    InvalidIdentifierStatement = 601,
    InvalidVariableStatement = 602,
    InvalidStringStatement = 603,
    InvalidCharacter = 604,
    InvalidNumberStatement = 605,
    InvalidToken = 606,
    InvalidBooleanExpression = 607,
    InvalidComperisionExpression = 607,
    // END Compiler Error

    // BEGIN BTREE
    NotFoundItem = 900,
    // END BTREE

};

class Error
{
    std::string m_description;
    ErrorType m_type;

public:
    Error(ErrorType type, std::string description);
};