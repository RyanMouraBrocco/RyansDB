#pragma once

#include <string>
#include <cctype>
#include <algorithm>
#include <stdexcept>

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
    DECIMAL,
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
    DATABASE,
    AND,
    OR,
    INNER,
    JOIN,
    ON,
    COLUMN,
    REFERENCES,
};

enum class NonTerminalToken
{
    LOGICAL_EXPRESSION,
    AND_EXPRESSION,
    TOKEN_EXPRESSION,
    COMPARISON_EXPRESSION,
    COMPARE_ACTION,
    FACTOR_EXPRESSION,
    IDENTIFIER_ATTRIBUTE,
    SELECT,
    SELECT_ITEM,
    FROM,
    JOIN,
    TABLE_NAME,
    INSERT,
    INSERT_VALUES,
    INSERT_VALUE,
    INSERT_IDENTIFIERS,
    UPDATE,
    UPDATE_SET,
    DELETE,
    CREATE_DATABASE,
    DROP_DATABASE,
    CREATE_TABLE,
    COLUMNS_DEFINITIONS,
    COLUMN_DEFINITION,
    COLUMN_TYPE,
    TEXT_TYPE,
    COLUMN_CONSTRAINTS,
    COLUMN_CONSTRAINT,
    ALTER_TABLE,
    ALTER_COMMAND,
    DROP_TABLE,
};

class TokenDefinition
{
private:
    Token m_token;
    std::string m_lowerCaseLexeme;
    std::string m_upperCaseLexeme;
    int m_lexemeLength;

public:
    TokenDefinition(Token token, std::string lexeme);
    TokenDefinition(Token token, std::string lowerCaseLexeme, std::string upperCaseLexeme);
    Token GetToken() const;
    std::string GetUpperCaseLexeme() const;
    int LexemeLength() const;
    bool IsValidCharacterInTheIndex(const char &value, const int &index) const;
};