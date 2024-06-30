#include "LogicalOperationParser.hpp"

std::optional<Error> LogicalOperationParser::ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;
    int depthNonTerminalToken = 0;

    do
    {
        symbolTable->AddNode(NonTerminalToken::LOGICAL_EXPRESSION);
        depthNonTerminalToken++;
        mustRepeat = false;

        errorResult = CheckAndOperation(symbolTable, tokens, index);
        if (errorResult.has_value())
            return errorResult;

        if (tokens[index].GetToken() == Token::OR)
        {
            errorResult = Consume(symbolTable, tokens, Token::OR, index);
            if (errorResult.has_value())
                return errorResult;

            mustRepeat = true;
        }
    } while (mustRepeat);

    for (int i = 0; i < depthNonTerminalToken; i++)
        symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> LogicalOperationParser::CheckAndOperation(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;
    int depthNonTerminalToken = 0;

    do
    {
        symbolTable->AddNode(NonTerminalToken::AND_EXPRESSION);
        depthNonTerminalToken++;
        mustRepeat = false;

        errorResult = CheckTokenExpression(symbolTable, tokens, index);
        if (errorResult.has_value())
            return errorResult;

        if (tokens[index].GetToken() == Token::AND)
        {
            errorResult = Consume(symbolTable, tokens, Token::AND, index);
            if (errorResult.has_value())
                return errorResult;

            mustRepeat = true;
        }
    } while (mustRepeat);

    for (int i = 0; i < depthNonTerminalToken; i++)
        symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> LogicalOperationParser::CheckTokenExpression(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    symbolTable->AddNode(NonTerminalToken::TOKEN_EXPRESSION);

    std::optional<Error> errorResult = std::nullopt;
    if (tokens[index].GetToken() == Token::NOT)
    {
        errorResult = Consume(symbolTable, tokens, Token::NOT, index);
        if (errorResult.has_value())
            return errorResult;

        if (tokens[index].GetToken() == Token::NULL_VALUE)
        {
            errorResult = Consume(symbolTable, tokens, Token::NULL_VALUE, index);
            if (errorResult.has_value())
                return errorResult;
        }

        errorResult = p_utilsParser->CheckFactorExpression(symbolTable, tokens, index);
        if (errorResult.has_value())
            return errorResult;
    }
    else if (tokens[index].GetToken() == Token::LEFT_PARENTHESIS)
    {
        errorResult = Consume(symbolTable, tokens, Token::LEFT_PARENTHESIS, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = this->ParserIntoParserTree(symbolTable, tokens, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(symbolTable, tokens, Token::RIGHT_PARENTHESIS, index);
        if (errorResult.has_value())
            return errorResult;
    }
    else
    {
        errorResult = CheckComparisonExpression(symbolTable, tokens, index);
        if (errorResult.has_value())
            return errorResult;
    }

    symbolTable->TierUp();
    return std::nullopt;
}

std::optional<Error> LogicalOperationParser::CheckComparisonExpression(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    symbolTable->AddNode(NonTerminalToken::COMPARISON_EXPRESSION);

    std::optional<Error> errorResult = std::nullopt;
    errorResult = p_utilsParser->CheckFactorExpression(symbolTable, tokens, index);
    if (errorResult.has_value())
        return errorResult;

    if (SymbolTable::IsComparisionToken(tokens[index].GetToken()))
    {
        errorResult = CheckCompareAction(symbolTable, tokens, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = p_utilsParser->CheckFactorExpression(symbolTable, tokens, index);
        if (errorResult.has_value())
            return errorResult;
    }

    symbolTable->TierUp();
    return std::nullopt;
}

std::optional<Error> LogicalOperationParser::CheckCompareAction(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    symbolTable->AddNode(NonTerminalToken::COMPARE_ACTION);
    std::optional<Error> errorResult = std::nullopt;

    if (SymbolTable::IsComparisionToken(tokens[index].GetToken()))
        errorResult = Consume(symbolTable, tokens, tokens[index].GetToken(), index);
    else
        errorResult = Error(ErrorType::InvalidToken, "It as expected a valid comparison token but receive a " + tokens[index].GetUpperCaseLexeme());

    symbolTable->TierUp();
    return errorResult;
}

std::shared_ptr<LogicalOperationParser> LogicalOperationParser::p_singleton = nullptr;
std::mutex LogicalOperationParser::m_mutex;

std::shared_ptr<LogicalOperationParser> LogicalOperationParser::GetInstance()
{
    if (p_singleton == nullptr)
    {
        m_mutex.lock();
        if (p_singleton == nullptr)
        {
            p_singleton = std::shared_ptr<LogicalOperationParser>();
        }
        m_mutex.unlock();
    }

    return p_singleton;
}