#include "SyntaxAnalyzer.hpp"

SyntaxAnalyzer::SyntaxAnalyzer(std::shared_ptr<SymbolTable> symbolTable) : p_symbolTable(symbolTable)
{
}

std::optional<Error> SyntaxAnalyzer::Execute()
{
    auto tokens = *p_symbolTable->GetSortTokens();
    int index = 0;

    while (index < tokens.size())
    {
        if (tokens[index].GetToken() == Token::SELECT)
            CheckSelectStatement(tokens, index);
    }
}

std::optional<Error> SyntaxAnalyzer::Consume(const Token &token, const Token &&expextedToken, int &index) const
{
    if (token != expextedToken)
        return Error(ErrorType::InvalidToken, "It was expected a <HERE> but was found a <HERE_2>");

    index++;
}

std::optional<Error> SyntaxAnalyzer::CheckSelectStatement(const std::vector<TokenDefinition> &tokens, int &index) const
{
    Consume(tokens[index].GetToken(), Token::SELECT, index);
    CheckSelectItems(tokens, index);
    Consume(tokens[index].GetToken(), Token::FROM, index);
    CheckFrom(tokens, index);

    if (tokens[index].GetToken() == Token::WHERE)
    {
        Consume(tokens[index].GetToken(), Token::WHERE, index);
        CheckLogicalExpression(tokens, index);
    }

    Consume(tokens[index].GetToken(), Token::SEMICOLON, index);
}

std::optional<Error> SyntaxAnalyzer::CheckSelectItems(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    do
    {
        mustRepeat = false;
        if (tokens[index].GetToken() == Token::ASTERISK)
            Consume(tokens[index].GetToken(), Token::ASTERISK, index);
        else
            Consume(tokens[index].GetToken(), Token::IDENTIFIER, index);

        if (tokens[index].GetToken() == Token::COMMA)
        {
            Consume(tokens[index].GetToken(), Token::COMMA, index);
            mustRepeat = true;
        }
    } while (mustRepeat);
}

std::optional<Error> SyntaxAnalyzer::CheckFrom(const std::vector<TokenDefinition> &tokens, int &index) const
{
    CheckTableName(tokens, index);
    if (tokens[index].GetToken() == Token::INNER)
        CheckJoin(tokens, index);
}

std::optional<Error> SyntaxAnalyzer::CheckJoin(const std::vector<TokenDefinition> &tokens, int &index) const
{
    do
    {
        Consume(tokens[index].GetToken(), Token::INNER, index);
        Consume(tokens[index].GetToken(), Token::JOIN, index);
        CheckTableName(tokens, index);
        Consume(tokens[index].GetToken(), Token::ON, index);
        CheckLogicalExpression(tokens, index);
    } while (tokens[index].GetToken() == Token::INNER);
}

std::optional<Error> SyntaxAnalyzer::CheckTableName(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    do
    {
        mustRepeat = false;
        Consume(tokens[index].GetToken(), Token::IDENTIFIER, index);
        if (tokens[index].GetToken() == Token::IDENTIFIER)
        {
            return Consume(tokens[index].GetToken(), Token::IDENTIFIER, index);
        }
    } while (mustRepeat);
}

std::optional<Error> SyntaxAnalyzer::CheckLogicalExpression(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    do
    {
        mustRepeat = false;
        CheckAndOperation(tokens, index);
        if (tokens[index].GetToken() == Token::OR)
        {
            Consume(tokens[index].GetToken(), Token::OR, index);
            mustRepeat = true;
        }
    } while (mustRepeat);
}

std::optional<Error> SyntaxAnalyzer::CheckAndOperation(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    do
    {
        mustRepeat = false;
        CheckTokenExpression(tokens, index);
        if (tokens[index].GetToken() == Token::AND)
        {
            Consume(tokens[index].GetToken(), Token::AND, index);
            mustRepeat = true;
        }
    } while (mustRepeat);
}

std::optional<Error> SyntaxAnalyzer::CheckTokenExpression(const std::vector<TokenDefinition> &tokens, int &index) const
{
    if (tokens[index].GetToken() == Token::NOT)
    {
        Consume(tokens[index].GetToken(), Token::NOT, index);
        // MORE HERE
    }
    else if (tokens[index].GetToken() == Token::LEFT_PARENTHESIS)
    {
        Consume(tokens[index].GetToken(), Token::LEFT_PARENTHESIS, index);
        CheckLogicalExpression(tokens, index);
        Consume(tokens[index].GetToken(), Token::RIGHT_PARENTHESIS, index);
    }
    else
        CheckComparisonExpression(tokens, index);
}

std::optional<Error> SyntaxAnalyzer::CheckComparisonExpression(const std::vector<TokenDefinition> &tokens, int &index) const
{
    CheckFactorExpression();
    if (IsValidComparisonToken())
    {
        CheckCompareAction();
        CheckFactorExpression();
    }
}

std::optional<Error> SyntaxAnalyzer::CheckCompareAction(const std::vector<TokenDefinition> &tokens, int &index) const
{
    
}

std::optional<Error> SyntaxAnalyzer::CheckFactorExpression(const std::vector<TokenDefinition> &tokens, int &index) const
{
    const validFactorsExpression = 
}

std::optional<Error> SyntaxAnalyzer::CheckIdentifierAttribute(const std::vector<TokenDefinition> &tokens, int &index) const
{
    Consume(tokens[index].GetToken(), Token::IDENTIFIER, index);
    if (tokens[index].GetToken() == Token::DOT)
    {
        Consume(tokens[index].GetToken(), Token::DOT, index);
        Consume(tokens[index].GetToken(), Token::IDENTIFIER, index);
    }
}