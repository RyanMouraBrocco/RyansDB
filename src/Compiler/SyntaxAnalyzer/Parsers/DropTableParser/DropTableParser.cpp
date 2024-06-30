#include "DropTableParser.hpp"

std::optional<Error> DropTableParser::ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::DROP_TABLE);

    errorResult = Consume(symbolTable, tokens, Token::DROP, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::TABLE, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::SEMICOLON, index);
    if (errorResult.has_value())
        return errorResult;

    symbolTable->TierUp();

    return std::nullopt;
}

std::shared_ptr<DropTableParser> DropTableParser::p_singleton = nullptr;
std::mutex DropTableParser::m_mutex;

std::shared_ptr<DropTableParser> DropTableParser::GetInstance()
{
    if (p_singleton == nullptr)
    {
        m_mutex.lock();
        if (p_singleton == nullptr)
        {
            p_singleton = std::make_shared<DropTableParser>();
        }
        m_mutex.unlock();
    }

    return p_singleton;
}