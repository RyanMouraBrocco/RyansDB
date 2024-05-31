#include "LexycalAnalyzer.hpp"

LexycalAnalyzer::LexycalAnalyzer(std::shared_ptr<SymbolTable> symbolTable) : p_symbolTable(symbolTable)
{
}

bool LexycalAnalyzer::Execute(std::string query)
{
    int length = query.length();
    int index = 0;

    while (index < length)
    {
        if (query[index] == ' ' || query[index] == '\n' || query[index] == '\t' || query[index] == '\r')
        {
            index++;
            continue;
        }

        if (isalpha(query[index]))
        {
            if (IsReservedStatement(query, index, length))
                continue;

            SaveIdentifierStatement(query, index);
        }
        else if (isdigit(query[index]))
        {
            SaveNumberStatement(query, index);
        }
        else if (query[index] == '@')
        {
            SaveVariableStatement(query, index);
        }
        else if (query[index] == '\'')
        {
            SaveStringStatement(query, index);
        }
        else if (IsValidSpecialCharacter(query, index))
        {
        }
        else
        {
        }
    }

    return true;
}

bool LexycalAnalyzer::IsEndTokenChar(const char &value) const
{
    return value == ' ' ||
           value == ' '; //
}

bool LexycalAnalyzer::SaveNumberStatement(const std::string &query, int &index)
{
    Token token = Token::INTEGER_NUMBER;

    std::stringstream stringBuilder;
    while (isdigit(query[index]))
    {
        stringBuilder << query[index];
        index++;
    }

    if (query[index] == '.')
    {
        stringBuilder << query[index];
        index++;
        while (isdigit(query[index]))
        {
            stringBuilder << query[index];
            index++;
        }

        token = Token::DECIMAL_NUMBER;
    }

    if (!IsEndTokenChar(query[index]))
        return false;

    p_symbolTable->AddToken(stringBuilder.str(), token);

    return true;
}

bool LexycalAnalyzer::IsValidSpecialCharacter(const std::string &query, int &index)
{
    std::map<char, Token> specialCharacterTokens =
        {
            {'*', Token::ASTERISK},
            {',', Token::COMMA},
            {'.', Token::DOT},
            {';', Token::SEMICOLON},
            {'=', Token::EQUAL},
            {'[', Token::LEFT_BRACKET},
            {']', Token::RIGHT_BRACKET},
            {'(', Token::LEFT_PARENTHESIS},
            {')', Token::RIGHT_PARENTHESIS},
        };

    if (specialCharacterTokens[query[index]] != specialCharacterTokens.end())
    {
        p_symbolTable->AddToken(str(1, query[index]), specialCharacterTokens[query[index]]);
        index++;
        return true;
    }
    else if (query[index] == '>' || query[index] == '<')
    {
        bool isGreaterThanSymbol = query[index] == '>';
        if (index + 1 < query.length() && query[index + 1] == '=')
        {
            p_symbolTable->AddToken(isGreaterThanSymbol ? ">=" : "<=", isGreaterThanSymbol ? Token::GREATER_OR_EQUAL_THAN : Token::LESS_OR_EQUAL_THAN);
            index += 2;
        }
        else
        {
            p_symbolTable->AddToken(str(1, query[index]), isGreaterThanSymbol ? Token::GREATER_THAN : Token::LESS_THAN);
            index++;
        }
    }

    return false;
}

bool LexycalAnalyzer::SaveVariableStatement(const std::string &query, int &index)
{
    int length = query.length();
    std::stringstream stringBuilder;

    stringBuilder << query[index];
    index++;

    while (index < length && !isdigit(query[index]) && !IsEndTokenChar(query[index]))
    {
        stringBuilder << query[index];
        index++;
    }

    p_symbolTable->AddToken(stringBuilder.str(), Token::VARIABLE);
    return true;
}

bool LexycalAnalyzer::SaveIdentifierStatement(const std::string &query, int &index)
{
    int length = query.length();
    std::stringstream stringBuilder;
    while (index < length && !isdigit(query[index]) && !IsEndTokenChar(query[index]))
    {
        stringBuilder << query[index];
        index++;
    }

    p_symbolTable->AddToken(stringBuilder.str(), Token::IDENTIFIER);
    return true;
}

bool LexycalAnalyzer::SaveStringStatement(const std::string &query, int &index)
{
    int length = query.length();
    index++; // skip first \'

    std::stringstream stringBuilder;
    while (index < length && query[index] != '\'')
    {
        stringBuilder << query[index];
        index++;
    }

    if (index >= length)
        return false;

    index++; // skip the second \'

    p_symbolTable->AddToken(stringBuilder.str(), Token::STRING);

    return true;
}

bool LexycalAnalyzer::IsReservedStatement(std::string &query, int &index, int &queryLength)
{
    if (IsStatement("SELECT", query, index))
    {
        p_symbolTable->AddToken("SELECT", Token::SELECT);
        index += 5;
        return true;
    }

    if (IsStatement("FROM", query, index))
    {
        p_symbolTable->AddToken("FROM", Token::FROM);
        index += 4;
        return true;
    }

    if (IsStatement("WHERE", query, index))
    {
        p_symbolTable->AddToken("WHERE", Token::WHERE);
        index += 5;
        return true;
    }

    if (IsStatement("NULL", query, index))
    {
        p_symbolTable->AddToken("NULL", Token::NULL_VALUE);
        index += 4;
        return true;
    }

    if (IsStatement("NOT", query, index))
    {
        p_symbolTable->AddToken("NOT", Token::NOT);
        index += 3;
        return true;
    }

    if (IsStatement("IS", query, index))
    {
        p_symbolTable->AddToken("IS", Token::IS);
        index += 2;
        return true;
    }

    if (IsStatement("IS", query, index))
    {
        p_symbolTable->AddToken("IS", Token::IS);
        index += 2;
        return true;
    }

    return false;
}

bool LexycalAnalyzer::IsStatement(const std::string &&expectedStatement,
                                  const std::string &query,
                                  const int &index) const
{
    int expextedStatementLen = expectedStatement.length();
    int queryLen = query.length();

    if (index + expextedStatementLen >= queryLen)
        return false;

    for (int8_t i = 0; i < expextedStatementLen; i++)
    {
        if (query[index + i] != expectedStatement[i])
            return false;
    }
    return index + expextedStatementLen + 1 >= queryLen || IsEndTokenChar(query[index + expextedStatementLen + 1]);
}