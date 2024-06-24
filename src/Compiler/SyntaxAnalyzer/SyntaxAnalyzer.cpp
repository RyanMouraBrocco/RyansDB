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

std::optional<Error> SyntaxAnalyzer::Consume(const std::vector<TokenDefinition> &tokens, const Token &&expextedToken, int &index) const
{
    if (index >= tokens.size())
        return Error(ErrorType::InvalidToken, "The command finished but it was expected a " + tokens[index].GetUpperCaseLexeme());

    if (tokens[index].GetToken() != expextedToken)
        return Error(ErrorType::InvalidToken, "It was expected a <HERE> but was found a " + tokens[index].GetUpperCaseLexeme());

    p_symbolTable->AddNode(tokens[index]);

    index++;

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckSelectStatement(const std::vector<TokenDefinition> &tokens, int &index) const
{
    p_symbolTable->AddNode(NonTerminalToken::SELECT);

    std::optional<Error> errorResult = std::nullopt;
    errorResult = Consume(tokens, Token::SELECT, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = CheckSelectItems(tokens, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::FROM, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = CheckFrom(tokens, index);
    if (errorResult.has_value())
        return errorResult;

    if (tokens[index].GetToken() == Token::WHERE)
    {
        errorResult = Consume(tokens, Token::WHERE, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = CheckLogicalExpression(tokens, index);
        if (errorResult.has_value())
            return errorResult;
    }

    errorResult = Consume(tokens, Token::SEMICOLON, index);
    if (errorResult.has_value())
        return errorResult;

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckSelectItems(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    p_symbolTable->AddNode(NonTerminalToken::SELECT_ITEM);
    do
    {
        mustRepeat = false;
        if (tokens[index].GetToken() == Token::ASTERISK)
        {
            errorResult = Consume(tokens, Token::ASTERISK, index);
            if (errorResult.has_value())
                return errorResult;
        }
        else
        {
            errorResult = Consume(tokens, Token::IDENTIFIER, index);
            if (errorResult.has_value())
                return errorResult;
        }

        if (tokens[index].GetToken() == Token::COMMA)
        {
            errorResult = Consume(tokens, Token::COMMA, index);
            if (errorResult.has_value())
                return errorResult;

            mustRepeat = true;
        }
    } while (mustRepeat);

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckFrom(const std::vector<TokenDefinition> &tokens, int &index) const
{
    p_symbolTable->AddNode(NonTerminalToken::FROM);

    std::optional<Error> errorResult = CheckTableName(tokens, index);
    if (errorResult.has_value())
        return errorResult;

    if (tokens[index].GetToken() == Token::INNER)
    {
        std::optional<Error> errorResult = CheckJoin(tokens, index);
        if (errorResult.has_value())
            return errorResult;
    }

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckJoin(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;
    int depthNonTerminalToken = 0;

    do
    {
        p_symbolTable->AddNode(NonTerminalToken::JOIN);
        depthNonTerminalToken++;

        errorResult = Consume(tokens, Token::INNER, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(tokens, Token::JOIN, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = CheckTableName(tokens, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(tokens, Token::ON, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = CheckLogicalExpression(tokens, index);
        if (errorResult.has_value())
            return errorResult;

    } while (tokens[index].GetToken() == Token::INNER);

    for (int i = 0; i < depthNonTerminalToken; i++)
        p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckTableName(const std::vector<TokenDefinition> &tokens, int &index) const
{
    p_symbolTable->AddNode(NonTerminalToken::TABLE_NAME);

    std::optional<Error> errorResult = Consume(tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    if (tokens[index].GetToken() == Token::IDENTIFIER)
    {
        errorResult = Consume(tokens, Token::IDENTIFIER, index);
        if (errorResult.has_value())
            return errorResult;
    }

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckLogicalExpression(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;
    int depthNonTerminalToken = 0;

    do
    {
        p_symbolTable->AddNode(NonTerminalToken::LOGICAL_EXPRESSION);
        depthNonTerminalToken++;
        mustRepeat = false;

        errorResult = CheckAndOperation(tokens, index);
        if (errorResult.has_value())
            return errorResult;

        if (tokens[index].GetToken() == Token::OR)
        {
            errorResult = Consume(tokens, Token::OR, index);
            if (errorResult.has_value())
                return errorResult;

            mustRepeat = true;
        }
    } while (mustRepeat);

    for (int i = 0; i < depthNonTerminalToken; i++)
        p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckAndOperation(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;
    int depthNonTerminalToken = 0;

    do
    {
        p_symbolTable->AddNode(NonTerminalToken::AND_EXPRESSION);
        depthNonTerminalToken++;
        mustRepeat = false;

        errorResult = CheckTokenExpression(tokens, index);
        if (errorResult.has_value())
            return errorResult;

        if (tokens[index].GetToken() == Token::AND)
        {
            errorResult = Consume(tokens, Token::AND, index);
            if (errorResult.has_value())
                return errorResult;

            mustRepeat = true;
        }
    } while (mustRepeat);

    for (int i = 0; i < depthNonTerminalToken; i++)
        p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckTokenExpression(const std::vector<TokenDefinition> &tokens, int &index) const
{
    p_symbolTable->AddNode(NonTerminalToken::TOKEN_EXPRESSION);

    std::optional<Error> errorResult = std::nullopt;
    if (tokens[index].GetToken() == Token::NOT)
    {
        errorResult = Consume(tokens, Token::NOT, index);
        if (errorResult.has_value())
            return errorResult;

        if (tokens[index].GetToken() == Token::NULL_VALUE)
        {
            errorResult = Consume(tokens, Token::NULL_VALUE, index);
            if (errorResult.has_value())
                return errorResult;
        }

        errorResult = CheckFactorExpression(tokens, index);
        if (errorResult.has_value())
            return errorResult;
    }
    else if (tokens[index].GetToken() == Token::LEFT_PARENTHESIS)
    {
        errorResult = Consume(tokens, Token::LEFT_PARENTHESIS, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = CheckLogicalExpression(tokens, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(tokens, Token::RIGHT_PARENTHESIS, index);
        if (errorResult.has_value())
            return errorResult;
    }
    else
    {
        errorResult = CheckComparisonExpression(tokens, index);
        if (errorResult.has_value())
            return errorResult;
    }

    p_symbolTable->TierUp();
    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckComparisonExpression(const std::vector<TokenDefinition> &tokens, int &index) const
{
    p_symbolTable->AddNode(NonTerminalToken::COMPARISON_EXPRESSION);

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

    p_symbolTable->TierUp();
    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckCompareAction(const std::vector<TokenDefinition> &tokens, int &index) const
{
    p_symbolTable->AddNode(NonTerminalToken::COMPARE_ACTION);
    std::optional<Error> errorResult = std::nullopt;

    if (SymbolTable::IsComparisionToken(tokens[index].GetToken()))
        errorResult = Consume(tokens, tokens[index].GetToken(), index);
    else
        errorResult = Error(ErrorType::InvalidToken, "It as expected a valid comparison token but receive a " + tokens[index].GetUpperCaseLexeme());

    p_symbolTable->TierUp();
    return errorResult;
}

std::optional<Error> SyntaxAnalyzer::CheckFactorExpression(const std::vector<TokenDefinition> &tokens, int &index) const
{
    p_symbolTable->AddNode(NonTerminalToken::FACTOR_EXPRESSION);
    std::optional<Error> errorResult = std::nullopt;

    if (tokens[index].GetToken() == Token::IDENTIFIER)
        errorResult = CheckIdentifierAttribute(tokens, index);
    else if (tokens[index].GetToken() == Token::NULL_VALUE)
        errorResult = Consume(tokens, Token::NULL_VALUE, index);
    else if (SymbolTable::IsFactorToken(tokens[index].GetToken()))
        errorResult = Consume(tokens, tokens[index].GetToken(), index);
    else
        errorResult = Error(ErrorType::InvalidToken, "It as expected a valid factor but receive a " + tokens[index].GetUpperCaseLexeme());

    p_symbolTable->TierUp();
    return errorResult;
}

std::optional<Error> SyntaxAnalyzer::CheckIdentifierAttribute(const std::vector<TokenDefinition> &tokens, int &index) const
{
    p_symbolTable->AddNode(NonTerminalToken::IDENTIFIER_ATTRIBUTE);
    std::optional<Error> errorResult = std::nullopt;
    errorResult = Consume(tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    if (tokens[index].GetToken() == Token::DOT)
    {
        errorResult = Consume(tokens, Token::DOT, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(tokens, Token::IDENTIFIER, index);
        if (errorResult.has_value())
            return errorResult;
    }

    p_symbolTable->TierUp();

    return std::nullopt;
}