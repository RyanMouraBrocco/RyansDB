#pragma once

#include <vector>
#include <tuple>
#include <string>

enum class Token
{
    IDENTIFIER,
    INTEGER_NUMBER,
    DECIMAL_NUMBER,
    STRING,
    SELECT,
    ASTERISK,
    COMMA,
    DOT,
    SEMICOLON,
    FROM,
    WHERE,
    GREATER_THAN,
    GREATER_OR_EQUAL_THAN,
    LESS_THAN,
    LESS_OR_EQUAL_THAN,
    EQUAL,
    NULL_VALUE,
    NOT,
    IS,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    VARIABLE,
    INSERT,
    UPDATE,
    DELETE,
    SET,
    INTO,
    VALUES,
    DECLARE,
    INT,
    NVARCHAR,
    VARCHAR,
    BIT,
    NCHAR,
    CHAR,
    PRIMARY,
    FOREIGN,
    KEY,
    CREATE,
    TABLE,
    DROP,
    ALTER,
    ADD,
    CONSTRAINT,
    UNIQUE,
    INDEX,
};

class SymbolTable
{
private:
    std::vector<std::tuple<std::string, Token>> m_tokens;

public:
    SymbolTable();
    void AddToken(const std::string value, const Token key);
};