#include "CreateDatabaseParser.hpp"

std::optional<Error> CreateDatabaseParser::ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::CREATE_DATABASE);

    errorResult = Consume(symbolTable, tokens, Token::CREATE, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::DATABASE, index);
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

std::shared_ptr<CreateDatabaseParser> CreateDatabaseParser::p_singleton = nullptr;
std::mutex CreateDatabaseParser::m_mutex;

std::shared_ptr<CreateDatabaseParser> CreateDatabaseParser::GetInstance()
{
    if (p_singleton == nullptr)
    {
        m_mutex.lock();
        if (p_singleton == nullptr)
        {
            p_singleton = std::make_shared<CreateDatabaseParser>();
        }
        m_mutex.unlock();
    }

    return p_singleton;
}