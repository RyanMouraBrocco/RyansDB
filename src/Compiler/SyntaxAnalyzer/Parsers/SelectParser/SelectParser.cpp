#include "SelectParser.hpp"

std::optional<Error> SelectParser::ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    symbolTable->AddNode(NonTerminalToken::SELECT);

    std::optional<Error> errorResult = std::nullopt;
    errorResult = Consume(symbolTable, tokens, Token::SELECT, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = CheckSelectItems(symbolTable, tokens, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::FROM, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = CheckFrom(symbolTable, tokens, index);
    if (errorResult.has_value())
        return errorResult;

    if (tokens[index].GetToken() == Token::WHERE)
    {
        errorResult = Consume(symbolTable, tokens, Token::WHERE, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = p_logicalExpressionParser->ParserIntoParserTree(symbolTable, tokens, index);
        if (errorResult.has_value())
            return errorResult;
    }

    errorResult = Consume(symbolTable, tokens, Token::SEMICOLON, index);
    if (errorResult.has_value())
        return errorResult;

    symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SelectParser::CheckSelectItems(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::SELECT_ITEM);
    do
    {
        mustRepeat = false;
        if (tokens[index].GetToken() == Token::ASTERISK)
        {
            errorResult = Consume(symbolTable, tokens, Token::ASTERISK, index);
            if (errorResult.has_value())
                return errorResult;
        }
        else
        {
            errorResult = Consume(symbolTable, tokens, Token::IDENTIFIER, index);
            if (errorResult.has_value())
                return errorResult;
        }

        if (tokens[index].GetToken() == Token::COMMA)
        {
            errorResult = Consume(symbolTable, tokens, Token::COMMA, index);
            if (errorResult.has_value())
                return errorResult;

            mustRepeat = true;
        }
    } while (mustRepeat);

    symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SelectParser::CheckFrom(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    symbolTable->AddNode(NonTerminalToken::FROM);

    std::optional<Error> errorResult = CheckTableName(symbolTable, tokens, index);
    if (errorResult.has_value())
        return errorResult;

    if (tokens[index].GetToken() == Token::INNER)
    {
        std::optional<Error> errorResult = CheckJoin(symbolTable, tokens, index);
        if (errorResult.has_value())
            return errorResult;
    }

    symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SelectParser::CheckJoin(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;
    int depthNonTerminalToken = 0;

    do
    {
        symbolTable->AddNode(NonTerminalToken::JOIN);
        depthNonTerminalToken++;

        errorResult = Consume(symbolTable, tokens, Token::INNER, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(symbolTable, tokens, Token::JOIN, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = CheckTableName(symbolTable, tokens, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(symbolTable, tokens, Token::ON, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = p_logicalExpressionParser->ParserIntoParserTree(symbolTable, tokens, index);
        if (errorResult.has_value())
            return errorResult;

    } while (tokens[index].GetToken() == Token::INNER);

    for (int i = 0; i < depthNonTerminalToken; i++)
        symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SelectParser::CheckTableName(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    symbolTable->AddNode(NonTerminalToken::TABLE_NAME);

    std::optional<Error> errorResult = Consume(symbolTable, tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    if (tokens[index].GetToken() == Token::IDENTIFIER)
    {
        errorResult = Consume(symbolTable, tokens, Token::IDENTIFIER, index);
        if (errorResult.has_value())
            return errorResult;
    }

    symbolTable->TierUp();

    return std::nullopt;
}

std::shared_ptr<SelectParser> SelectParser::p_singleton = nullptr;
std::mutex SelectParser::m_mutex;

std::shared_ptr<SelectParser> SelectParser::GetInstance()
{
    if (p_singleton == nullptr)
    {
        m_mutex.lock();
        if (p_singleton == nullptr)
        {
            p_singleton = std::shared_ptr<SelectParser>();
        }
        m_mutex.unlock();
    }

    return p_singleton;
}