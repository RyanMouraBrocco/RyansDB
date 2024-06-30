#include "AlterTableParser.hpp"

std::optional<Error> AlterTableParser::ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::ALTER_TABLE);

    errorResult = Consume(symbolTable, tokens, Token::ALTER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::TABLE, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = CheckAlterCommand(symbolTable, tokens, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::SEMICOLON, index);
    if (errorResult.has_value())
        return errorResult;

    symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> AlterTableParser::CheckAlterCommand(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::ALTER_COMMAND);

    if (tokens[index].GetToken() == Token::ADD)
    {
        errorResult = Consume(symbolTable, tokens, Token::ADD, index);
        if (errorResult.has_value())
            return errorResult;

        if (tokens[index].GetToken() == Token::FOREIGN)
        {
            errorResult = ConsumeAddForeignKey(symbolTable, tokens, index);
            if (errorResult.has_value())
                return errorResult;
        }
        else
        {
            errorResult = Consume(symbolTable, tokens, Token::COLUMN, index);
            if (errorResult.has_value())
                return errorResult;

            errorResult = CheckColumnDefinition(symbolTable, tokens, index);
            if (errorResult.has_value())
                return errorResult;
        }
    }
    else if (tokens[index].GetToken() == Token::DROP)
    {
        errorResult = Consume(symbolTable, tokens, Token::DROP, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(symbolTable, tokens, Token::COLUMN, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(symbolTable, tokens, Token::IDENTIFIER, index);
        if (errorResult.has_value())
            return errorResult;
    }
    else
        return Error(ErrorType::InvalidToken, "It was expected a valid insertable value");

    symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> AlterTableParser::ConsumeAddForeignKey(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;
    errorResult = Consume(symbolTable, tokens, Token::FOREIGN, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::KEY, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::REFERENCES, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::LEFT_PARENTHESIS, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(symbolTable, tokens, Token::RIGHT_PARENTHESIS, index);
    if (errorResult.has_value())
        return errorResult;

    return std::nullopt;
}

std::shared_ptr<AlterTableParser> AlterTableParser::p_singleton = nullptr;
std::mutex AlterTableParser::m_mutex;

std::shared_ptr<AlterTableParser> AlterTableParser::GetInstance()
{
    if (p_singleton == nullptr)
    {
        m_mutex.lock();
        if (p_singleton == nullptr)
        {
            p_singleton = std::shared_ptr<AlterTableParser>();
        }
        m_mutex.unlock();
    }

    return p_singleton;
}