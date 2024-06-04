#pragma once

#include <vector>
#include <tuple>
#include <string>
#include <map>
#include <variant>
#include <memory>
#include <stdexcept>
#include "../../Error/error.hpp"

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

class LexemeTokenDefinition
{
private:
    Token m_token;
    std::string m_lowerCaseLexeme;
    std::string m_upperCaseLexeme;
    int m_lexemeLength;

public:
    LexemeTokenDefinition(Token token, std::string lowerCaseLexeme, std::string upperCaseLexeme);
    Token GetToken() const;
    std::string GetUpperCaseLexeme() const;
    int LexemeLength() const;
    bool IsValidCharacterInTheIndex(const char &value, const int &index) const;
};

class SymbolTable
{
private:
    std::vector<std::tuple<std::string, Token>> m_tokens;

    static std::shared_ptr<std::vector<LexemeTokenDefinition>> m_reservedStatements;
    static std::map<char, Token> m_specialCharacterTokens;

public:
    SymbolTable();
    void AddToken(const std::string value, const Token key);

    static std::variant<Token, Error> GetSpecialCharacterToken(const char &value);
    static std::shared_ptr<std::vector<LexemeTokenDefinition>> GetReservedStatementsDefinitions();
};