#include "InsertParser.hpp"

std::optional<Error> InsertParser::ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::INSERT);

    errorResult = Consume(symbolTable, tokens, Token::INSERT, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::INTO, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::VALUES, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::LEFT_PARENTHESIS, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = CheckInsertValues(symbolTable, tokens, index);
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

std::optional<Error> InsertParser::CheckInsertValues(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::INSERT_VALUES);
    do
    {
        mustRepeat = false;

        errorResult = CheckInsertValue(symbolTable, tokens, index);
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

std::optional<Error> InsertParser::CheckInsertValue(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    switch (tokens[index].GetToken())
    {
    case Token::VARIABLE:
    case Token::STRING:
    case Token::INTEGER_NUMBER:
    case Token::DECIMAL_NUMBER:
    case Token::NULL_VALUE:
        return Consume(symbolTable, tokens, tokens[index].GetToken(), index);
    default:
        return Error(ErrorType::InvalidToken, "It was expected a valid insertable value");
    }
}

std::optional<Error> InsertParser::CheckInsertIdentifiers(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::INSERT_IDENTIFIERS);
    do
    {
        mustRepeat = false;

        errorResult = Consume(symbolTable, tokens, Token::IDENTIFIER, index);
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

std::shared_ptr<InsertParser> InsertParser::p_singleton = nullptr;
std::mutex InsertParser::m_mutex;

std::shared_ptr<InsertParser> InsertParser::GetInstance()
{
    if (p_singleton == nullptr)
    {
        m_mutex.lock();
        if (p_singleton == nullptr)
        {
            p_singleton = std::shared_ptr<InsertParser>();
        }
        m_mutex.unlock();
    }

    return p_singleton;
}