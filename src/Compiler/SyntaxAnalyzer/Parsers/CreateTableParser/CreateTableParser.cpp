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

        errorResult = CheckColumnDefinition(symbolTable, tokens, index);
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

std::optional<Error> CreateTableParser::CheckColumnDefinition(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::COLUMN_DEFINITION);

    if (IsColumnConstraint(tokens, index))
        CheckColumnContraints(symbolTable, tokens, index);

    CheckTextType(symbolTable, tokens, index);

    if (IsColumnConstraint(tokens, index))
        CheckColumnContraints(symbolTable, tokens, index);

    symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> CreateTableParser::CheckColumnType(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::COLUMN_TYPE);

    Token currentToken = tokens[index].GetToken();
    if (currentToken == Token::INTEGER_NUMBER || currentToken == Token::DECIMAL_NUMBER)
    {
        errorResult = Consume(symbolTable, tokens, tokens[index].GetToken(), index);
        if (errorResult.has_value())
            return errorResult;
    }
    else
    {
        errorResult = CheckTextType(symbolTable, tokens, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(symbolTable, tokens, Token::LEFT_PARENTHESIS, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(symbolTable, tokens, Token::INTEGER_NUMBER, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(symbolTable, tokens, Token::RIGHT_PARENTHESIS, index);
        if (errorResult.has_value())
            return errorResult;
    }

    symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> CreateTableParser::CheckTextType(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::TEXT_TYPE);

    switch (tokens[index].GetToken())
    {
    case Token::NVARCHAR:
    case Token::VARCHAR:
    case Token::CHAR:
    case Token::NCHAR:
        return Consume(symbolTable, tokens, tokens[index].GetToken(), index);
    default:
        return Error(ErrorType::InvalidToken, "It was expected a valid insertable value");
    }

    symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> CreateTableParser::CheckColumnContraints(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    symbolTable->AddNode(NonTerminalToken::COLUMN_CONSTRAINTS);

    do
    {
        auto errorResult = CheckColumnContraint(symbolTable, tokens, index);
        if (errorResult.has_value())
            return errorResult;
    } while (IsColumnConstraint(tokens, index));

    symbolTable->TierUp();

    return std::nullopt;
}

bool CreateTableParser::IsColumnConstraint(const std::vector<TokenDefinition> &tokens, const int &index) const
{
    Token currentToken = tokens[index].GetToken();
    return currentToken == Token::PRIMARY ||
           currentToken == Token::NULL_VALUE ||
           currentToken == Token::NOT ||
           currentToken == Token::UNIQUE;
}

std::optional<Error> CreateTableParser::CheckColumnContraint(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::COLUMN_CONSTRAINT);

    if (tokens[index].GetToken() == Token::PRIMARY)
    {
        errorResult = Consume(symbolTable, tokens, Token::PRIMARY, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(symbolTable, tokens, Token::KEY, index);
        if (errorResult.has_value())
            return errorResult;
    }
    else if (tokens[index].GetToken() == Token::NOT)
    {
        errorResult = Consume(symbolTable, tokens, Token::NOT, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(symbolTable, tokens, Token::NULL_VALUE, index);
        if (errorResult.has_value())
            return errorResult;
    }
    else if (tokens[index].GetToken() == Token::UNIQUE || tokens[index].GetToken() == Token::NULL_VALUE)
    {
        errorResult = Consume(symbolTable, tokens, tokens[index].GetToken(), index);
        if (errorResult.has_value())
            return errorResult;
    }
    else
        return Error(ErrorType::InvalidToken, "It was expected a valid insertable value");

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