#include "CreateTableParser.hpp"

std::optional<Error> CreateTableParser::ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::CREATE_TABLE);

    errorResult = Consume(symbolTable, tokens, Token::CREATE, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::TABLE, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::LEFT_PARENTHESIS, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = CheckColumnsDefinitions(symbolTable, tokens, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::RIGHT_PARENTHESIS, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::SEMICOLON, index);
    if (errorResult.has_value())
        return errorResult;

    symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> CreateTableParser::CheckColumnsDefinitions(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::COLUMNS_DEFINITIONS);

    do
    {
        mustRepeat = false;

        errorResult = Consume(symbolTable, tokens, Token::IDENTIFIER, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = p_utilsParser->CheckColumnDefinition(symbolTable, tokens, index);
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

std::shared_ptr<CreateTableParser> CreateTableParser::p_singleton = nullptr;
std::mutex CreateTableParser::m_mutex;

std::shared_ptr<CreateTableParser> CreateTableParser::GetInstance()
{
    if (p_singleton == nullptr)
    {
        m_mutex.lock();
        if (p_singleton == nullptr)
        {
            p_singleton = std::shared_ptr<CreateTableParser>();
        }
        m_mutex.unlock();
    }

    return p_singleton;
}