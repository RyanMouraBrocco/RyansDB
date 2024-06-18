#include "SyntaxAnalyzer.hpp"

SyntaxAnalyzer::SyntaxAnalyzer(std::shared_ptr<SymbolTable> symbolTable) : p_symbolTable(symbolTable)
{
}

std::optional<Error> SyntaxAnalyzer::Execute()
{
    auto tokens = *p_symbolTable->GetSortTokens();
    int index = 0;

    std::optional<Error> errorResult = std::nullopt;
    while (index < tokens.size())
    {
        if (tokens[index].GetToken() == Token::SELECT)
            errorResult = CheckSelectStatement(tokens, index);

        if (errorResult != std::nullopt)
            return errorResult;
    }

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::Consume(const TokenDefinition &tokenDefinition, const Token &&expextedToken, int &index) const
{
    if (tokenDefinition.GetToken() != expextedToken)
        return Error(ErrorType::InvalidToken, "It was expected a <HERE> but was found a " + tokenDefinition.GetUpperCaseLexeme());

    index++;

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckSelectStatement(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;
    errorResult = Consume(tokens[index], Token::SELECT, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = CheckSelectItems(tokens, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens[index], Token::FROM, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = CheckFrom(tokens, index);
    if (errorResult.has_value())
        return errorResult;

    if (tokens[index].GetToken() == Token::WHERE)
    {
        errorResult = Consume(tokens[index], Token::WHERE, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = CheckLogicalExpression(tokens, index);
        if (errorResult.has_value())
            return errorResult;
    }

    return Consume(tokens[index], Token::SEMICOLON, index);
}

std::optional<Error> SyntaxAnalyzer::CheckSelectItems(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    do
    {
        mustRepeat = false;
        if (tokens[index].GetToken() == Token::ASTERISK)
        {
            errorResult = Consume(tokens[index], Token::ASTERISK, index);
            if (errorResult.has_value())
                return errorResult;
        }
        else
        {
            errorResult = Consume(tokens[index], Token::IDENTIFIER, index);
            if (errorResult.has_value())
                return errorResult;
        }

        if (tokens[index].GetToken() == Token::COMMA)
        {
            errorResult = Consume(tokens[index], Token::COMMA, index);
            if (errorResult.has_value())
                return errorResult;

            mustRepeat = true;
        }
    } while (mustRepeat);

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckFrom(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = CheckTableName(tokens, index);
    if (errorResult.has_value())
        return errorResult;

    if (tokens[index].GetToken() == Token::INNER)
        return CheckJoin(tokens, index);

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckJoin(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;
    do
    {
        errorResult = Consume(tokens[index], Token::INNER, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(tokens[index], Token::JOIN, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = CheckTableName(tokens, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(tokens[index], Token::ON, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = CheckLogicalExpression(tokens, index);
        if (errorResult.has_value())
            return errorResult;

    } while (tokens[index].GetToken() == Token::INNER);

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckTableName(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = Consume(tokens[index], Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    if (tokens[index].GetToken() == Token::IDENTIFIER)
    {
        errorResult = Consume(tokens[index], Token::IDENTIFIER, index);
        if (errorResult.has_value())
            return errorResult;
    }

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckLogicalExpression(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    do
    {
        mustRepeat = false;
        errorResult = CheckAndOperation(tokens, index);
        if (errorResult.has_value())
            return errorResult;

        if (tokens[index].GetToken() == Token::OR)
        {
            errorResult = Consume(tokens[index], Token::OR, index);
            if (errorResult.has_value())
                return errorResult;

            mustRepeat = true;
        }
    } while (mustRepeat);

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckAndOperation(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    do
    {
        mustRepeat = false;
        errorResult = CheckTokenExpression(tokens, index);
        if (errorResult.has_value())
            return errorResult;

        if (tokens[index].GetToken() == Token::AND)
        {
            errorResult = Consume(tokens[index], Token::AND, index);
            if (errorResult.has_value())
                return errorResult;

            mustRepeat = true;
        }
    } while (mustRepeat);

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckTokenExpression(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;
    if (tokens[index].GetToken() == Token::NOT)
    {
        errorResult = Consume(tokens[index], Token::NOT, index);
        if (errorResult.has_value())
            return errorResult;

        if (tokens[index].GetToken() == Token::NULL_VALUE)
        {
            errorResult = Consume(tokens[index], Token::NULL_VALUE, index);
            if (errorResult.has_value())
                return errorResult;
        }

        return CheckFactorExpression(tokens, index);
    }
    else if (tokens[index].GetToken() == Token::LEFT_PARENTHESIS)
    {
        errorResult = Consume(tokens[index], Token::LEFT_PARENTHESIS, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = CheckLogicalExpression(tokens, index);
        if (errorResult.has_value())
            return errorResult;

        return Consume(tokens[index], Token::RIGHT_PARENTHESIS, index);
    }
    else
        return CheckComparisonExpression(tokens, index);
}

std::optional<Error> SyntaxAnalyzer::CheckComparisonExpression(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;
    errorResult = CheckFactorExpression(tokens, index);
    if (errorResult.has_value())
        return errorResult;

    if (SymbolTable::IsComparisionToken(tokens[index].GetToken()))
    {
        errorResult = CheckCompareAction(tokens, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = CheckFactorExpression(tokens, index);
        if (errorResult.has_value())
            return errorResult;
    }

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckCompareAction(const std::vector<TokenDefinition> &tokens, int &index) const
{
    if (SymbolTable::IsComparisionToken(tokens[index].GetToken()))
        return Consume(tokens[index], tokens[index].GetToken(), index);

    return Error(ErrorType::InvalidToken, "It as expected a valid comparison token but receive a " + tokens[index].GetUpperCaseLexeme());
}

std::optional<Error> SyntaxAnalyzer::CheckFactorExpression(const std::vector<TokenDefinition> &tokens, int &index) const
{
    if (tokens[index].GetToken() == Token::IDENTIFIER)
        return CheckIdentifierAttribute(tokens, index);
    else if (tokens[index].GetToken() == Token::NULL_VALUE)
        return Consume(tokens[index], Token::NULL_VALUE, index);
    else if (SymbolTable::IsFactorToken(tokens[index].GetToken()))
        return Consume(tokens[index], tokens[index].GetToken(), index);

    return Error(ErrorType::InvalidToken, "It as expected a valid factor but receive a " + tokens[index].GetUpperCaseLexeme());
}

std::optional<Error> SyntaxAnalyzer::CheckIdentifierAttribute(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;
    errorResult = Consume(tokens[index], Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    if (tokens[index].GetToken() == Token::DOT)
    {
        errorResult = Consume(tokens[index], Token::DOT, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(tokens[index], Token::IDENTIFIER, index);
        if (errorResult.has_value())
            return errorResult;
    }

    return std::nullopt;
}