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
        }
        else if (isdigit(query[index]))
        {
        }
        else if (query[index] == '@')
        {
        }
        else if (query[index] == '\'')
        {
            std::stringstream stringBuilder;
            while (index < length || query[index] != '\'')
            {
                stringBuilder << query[index];
                index++;
            }

            if (index >= length)
                return false;

            index++;

            p_symbolTable->AddToken(stringBuilder.str(), Token::STRING);
        }
        else if (IsSpecialCharacter())
        {
        }
        else
        {
        }
    }

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