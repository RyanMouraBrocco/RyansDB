#include "UtilsParser.hpp"

std::optional<Error> UtilsParser::CheckFactorExpression(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    symbolTable->AddNode(NonTerminalToken::FACTOR_EXPRESSION);
    std::optional<Error> errorResult = std::nullopt;

    if (tokens[index].GetToken() == Token::IDENTIFIER)
        errorResult = CheckIdentifierAttribute(symbolTable, tokens, index);
    else if (tokens[index].GetToken() == Token::NULL_VALUE)
        errorResult = AddInAbstractSyntaxTree(symbolTable, tokens, Token::NULL_VALUE, index);
    else if (SymbolTable::IsFactorToken(tokens[index].GetToken()))
        errorResult = AddInAbstractSyntaxTree(symbolTable, tokens, tokens[index].GetToken(), index);
    else
        errorResult = Error(ErrorType::InvalidToken, "It as expected a valid factor but receive a " + tokens[index].GetUpperCaseLexeme());

    symbolTable->TierUp();
    return errorResult;
}

std::optional<Error> UtilsParser::CheckIdentifierAttribute(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    symbolTable->AddNode(NonTerminalToken::IDENTIFIER_ATTRIBUTE);
    std::optional<Error> errorResult = std::nullopt;
    errorResult = AddInAbstractSyntaxTree(symbolTable, tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    if (tokens[index].GetToken() == Token::DOT)
    {
        errorResult = Consume(symbolTable, tokens, Token::DOT, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = AddInAbstractSyntaxTree(symbolTable, tokens, Token::IDENTIFIER, index);
        if (errorResult.has_value())
            return errorResult;
    }

    symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> UtilsParser::CheckColumnDefinition(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::COLUMN_DEFINITION);

    if (IsColumnConstraint(tokens, index))
    {
        errorResult = CheckColumnContraints(symbolTable, tokens, index);
        if (errorResult.has_value())
            return errorResult;
    }

    errorResult = CheckColumnType(symbolTable, tokens, index);
    if (errorResult.has_value())
        return errorResult;

    if (IsColumnConstraint(tokens, index))
    {
        errorResult = CheckColumnContraints(symbolTable, tokens, index);
        if (errorResult.has_value())
            return errorResult;
    }

    symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> UtilsParser::CheckColumnType(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::COLUMN_TYPE);

    Token currentToken = tokens[index].GetToken();
    if (currentToken == Token::INT || currentToken == Token::DECIMAL)
    {
        errorResult = AddInAbstractSyntaxTree(symbolTable, tokens, tokens[index].GetToken(), index);
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

        errorResult = AddInAbstractSyntaxTree(symbolTable, tokens, Token::INTEGER_NUMBER, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(symbolTable, tokens, Token::RIGHT_PARENTHESIS, index);
        if (errorResult.has_value())
            return errorResult;
    }

    symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> UtilsParser::CheckTextType(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::TEXT_TYPE);

    switch (tokens[index].GetToken())
    {
    case Token::NVARCHAR:
    case Token::VARCHAR:
    case Token::CHAR:
    case Token::NCHAR:
        errorResult = AddInAbstractSyntaxTree(symbolTable, tokens, tokens[index].GetToken(), index);
        break;
    default:
        errorResult = Error(ErrorType::InvalidToken, "It was expected a valid insertable value");
        break;
    }

    symbolTable->TierUp();

    return errorResult;
}

std::optional<Error> UtilsParser::CheckColumnContraints(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
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

bool UtilsParser::IsColumnConstraint(const std::vector<TokenDefinition> &tokens, const int &index) const
{
    Token currentToken = tokens[index].GetToken();
    return currentToken == Token::PRIMARY ||
           currentToken == Token::NULL_VALUE ||
           currentToken == Token::NOT ||
           currentToken == Token::UNIQUE;
}

std::optional<Error> UtilsParser::CheckColumnContraint(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    symbolTable->AddNode(NonTerminalToken::COLUMN_CONSTRAINT);

    if (tokens[index].GetToken() == Token::PRIMARY)
    {
        errorResult = AddInAbstractSyntaxTree(symbolTable, tokens, Token::PRIMARY, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = AddInAbstractSyntaxTree(symbolTable, tokens, Token::KEY, index);
        if (errorResult.has_value())
            return errorResult;
    }
    else if (tokens[index].GetToken() == Token::NOT)
    {
        errorResult = AddInAbstractSyntaxTree(symbolTable, tokens, Token::NOT, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = AddInAbstractSyntaxTree(symbolTable, tokens, Token::NULL_VALUE, index);
        if (errorResult.has_value())
            return errorResult;
    }
    else if (tokens[index].GetToken() == Token::UNIQUE || tokens[index].GetToken() == Token::NULL_VALUE)
    {
        errorResult = AddInAbstractSyntaxTree(symbolTable, tokens, tokens[index].GetToken(), index);
        if (errorResult.has_value())
            return errorResult;
    }
    else
        return Error(ErrorType::InvalidToken, "It was expected a valid insertable value");

    symbolTable->TierUp();

    return std::nullopt;
}

std::shared_ptr<UtilsParser> UtilsParser::p_singleton = nullptr;
std::mutex UtilsParser::m_mutex;

std::shared_ptr<UtilsParser> UtilsParser::GetInstance()
{
    if (p_singleton == nullptr)
    {
        m_mutex.lock();
        if (p_singleton == nullptr)
        {
            p_singleton = std::make_shared<UtilsParser>();
        }
        m_mutex.unlock();
    }

    return p_singleton;
}