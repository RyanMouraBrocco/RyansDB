#include "UpdateParser.hpp"

std::optional<Error> UpdateParser::ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::UPDATE);

    errorResult = Consume(symbolTable, tokens, Token::UPDATE, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::SET, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = CheckUpdateSet(symbolTable, tokens, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::WHERE, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = p_logicalExpressionParser->ParserIntoParserTree(symbolTable, tokens, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::SEMICOLON, index);
    if (errorResult.has_value())
        return errorResult;

    symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> UpdateParser::CheckUpdateSet(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::UPDATE_SET);

    do
    {
        mustRepeat = false;

        errorResult = p_utilsParser->CheckIdentifierAttribute(symbolTable, tokens, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(symbolTable, tokens, Token::EQUAL, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = p_utilsParser->CheckFactorExpression(symbolTable, tokens, index);
        if (errorResult.has_value())
            return errorResult;

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

std::shared_ptr<UpdateParser> UpdateParser::p_singleton = nullptr;
std::mutex UpdateParser::m_mutex;

std::shared_ptr<UpdateParser> UpdateParser::GetInstance()
{
    if (p_singleton == nullptr)
    {
        m_mutex.lock();
        if (p_singleton == nullptr)
        {
            p_singleton = std::shared_ptr<UpdateParser>();
        }
        m_mutex.unlock();
    }

    return p_singleton;
}