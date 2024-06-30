#include "DeleteParser.hpp"

std::optional<Error> DeleteParser::ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::DELETE);

    errorResult = Consume(symbolTable, tokens, Token::DELETE, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::IDENTIFIER, index);
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

std::shared_ptr<DeleteParser> DeleteParser::p_singleton = nullptr;
std::mutex DeleteParser::m_mutex;

std::shared_ptr<DeleteParser> DeleteParser::GetInstance()
{
    if (p_singleton == nullptr)
    {
        m_mutex.lock();
        if (p_singleton == nullptr)
        {
            p_singleton = std::make_shared<DeleteParser>();
        }
        m_mutex.unlock();
    }

    return p_singleton;
}