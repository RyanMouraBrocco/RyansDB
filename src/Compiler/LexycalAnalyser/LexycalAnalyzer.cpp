#include "LexycalAnalyzer.hpp"

LexycalAnalyzer::LexycalAnalyzer(std::shared_ptr<SymbolTable> symbolTable) : p_symbolTable(symbolTable)
{
}

std::optional<Error> LexycalAnalyzer::Execute(std::string query)
{
    int length = query.length();
    int index = 0;

    while (index < length)
    {
        if (IsSkippableChar(query[index]))
        {
            index++;
            continue;
        }

        std::optional<Error> errorResult = std::nullopt;

        if (isalpha(query[index]))
            errorResult = SaveAlphaStatement(query, index);
        else if (isdigit(query[index]))
            errorResult = SaveNumberStatement(query, index);
        else if (query[index] == '@')
            errorResult = SaveVariableStatement(query, index);
        else if (query[index] == '\'')
            errorResult = SaveStringStatement(query, index);
        else
            errorResult = SaveSpecialCharacterStatement(query, index);

        if (errorResult.has_value())
            return errorResult.value();
    }

    return std::nullopt;
}

bool LexycalAnalyzer::IsSkippableChar(const char &value) const
{
    return value == ' ' || value == '\n' || value == '\t' || value == '\r';
}

bool LexycalAnalyzer::IsEndTokenChar(const char &value) const
{
    return value == ' ' ||
           value == '\n' ||
           value == '\r';
}

bool LexycalAnalyzer::IsStatement(const std::string &expectedStatement,
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
    return index + expextedStatementLen >= queryLen || IsEndTokenChar(query[index + expextedStatementLen]);
}

std::optional<Error> LexycalAnalyzer::SaveAlphaStatement(const std::string &query, int &index)
{
    auto errorResult = SaveReservedStatement(query, index);
    if (errorResult.has_value())
        return SaveIdentifierStatement(query, index);

    return std::nullopt;
}

std::optional<Error> LexycalAnalyzer::SaveReservedStatement(const std::string &query, int &index)
{
    std::vector<std::tuple<std::string, Token>> reservedStatements =
        {
            std::tuple<std::string, Token>("SELECT", Token::SELECT),
            std::tuple<std::string, Token>("FROM", Token::FROM),
            std::tuple<std::string, Token>("WHERE", Token::WHERE),
            std::tuple<std::string, Token>("NULL", Token::NULL_VALUE),
            std::tuple<std::string, Token>("NOT", Token::NOT),
            std::tuple<std::string, Token>("IS", Token::IS),
        };

    for (std::tuple<std::string, Token> statement : reservedStatements)
    {
        std::string text = std::get<0>(statement);
        if (IsStatement(text, query, index))
        {
            Token token = std::get<1>(statement);
            p_symbolTable->AddToken(text, token);
            index += text.length();
            return std::nullopt;
        }
    }

    return Error(ErrorType::InvalidReservedStatement, "Invalid reserved statement");
}

std::optional<Error> LexycalAnalyzer::SaveIdentifierStatement(const std::string &query, int &index)
{
    int length = query.length();
    std::stringstream stringBuilder;
    while (index < length && isalpha(query[index]) && !IsEndTokenChar(query[index]))
    {
        stringBuilder << query[index];
        index++;
    }

    if (index < length && !IsEndTokenChar(query[index]))
        return Error(ErrorType::InvalidIdentifierStatement, "Invalid identifier close the sentence: " + query[index]);

    p_symbolTable->AddToken(stringBuilder.str(), Token::IDENTIFIER);
    return std::nullopt;
}

std::optional<Error> LexycalAnalyzer::SaveNumberStatement(const std::string &query, int &index)
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

        if (!isdigit(query[index]))
            return Error(ErrorType::InvalidNumberStatement, "Invalid number close sentence: " + query[index]);

        while (isdigit(query[index]))
        {
            stringBuilder << query[index];
            index++;
        }

        token = Token::DECIMAL_NUMBER;
    }

    if (index < query.length() && !IsEndTokenChar(query[index]))
        return Error(ErrorType::InvalidNumberStatement, "Invalid number close sentence: " + query[index]);

    p_symbolTable->AddToken(stringBuilder.str(), token);

    return std::nullopt;
}

std::optional<Error> LexycalAnalyzer::SaveVariableStatement(const std::string &query, int &index)
{
    int length = query.length();
    std::stringstream stringBuilder;

    stringBuilder << query[index];
    index++;

    while (index < length && isalpha(query[index]) && !IsEndTokenChar(query[index]))
    {
        stringBuilder << query[index];
        index++;
    }

    if (index < length && !IsEndTokenChar(query[index]))
        return Error(ErrorType::InvalidVariableStatement, "Invalid variable close the sentence: " + query[index]);

    p_symbolTable->AddToken(stringBuilder.str(), Token::VARIABLE);
    return std::nullopt;
}

std::optional<Error> LexycalAnalyzer::SaveStringStatement(const std::string &query, int &index)
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
        return Error(ErrorType::InvalidStringStatement, "Missing \' to finish the string");

    index++; // skip the second \'

    p_symbolTable->AddToken(stringBuilder.str(), Token::STRING);

    return std::nullopt;
}

std::optional<Error> LexycalAnalyzer::SaveSpecialCharacterStatement(const std::string &query, int &index)
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

    if (specialCharacterTokens.find(query[index]) != specialCharacterTokens.end())
    {
        p_symbolTable->AddToken(std::string(1, query[index]), specialCharacterTokens[query[index]]);
        index++;
        return std::nullopt;
    }
    else if (query[index] == '>' || query[index] == '<')
        return SaveGreaterOrLessThanStatement(query, index);

    return Error(ErrorType::InvalidCharacter, "Invalid char in sentence: " + query[index]);
}

std::optional<Error> LexycalAnalyzer::SaveGreaterOrLessThanStatement(const std::string &query, int &index)
{
    bool isGreaterThanSymbol = query[index] == '>';
    if (index + 1 < query.length() && query[index + 1] == '=')
    {
        p_symbolTable->AddToken(isGreaterThanSymbol ? ">=" : "<=", isGreaterThanSymbol ? Token::GREATER_OR_EQUAL_THAN : Token::LESS_OR_EQUAL_THAN);
        index += 2;
    }
    else
    {
        p_symbolTable->AddToken(std::string(1, query[index]), isGreaterThanSymbol ? Token::GREATER_THAN : Token::LESS_THAN);
        index++;
    }

    return std::nullopt;
}