#include "SyntaxAnalyzer.hpp"

SyntaxAnalyzer::SyntaxAnalyzer(std::shared_ptr<SymbolTable> symbolTable) : p_symbolTable(symbolTable)
{
}

std::optional<Error> SyntaxAnalyzer::Execute()
{
    auto tokens = *p_symbolTable->GetSortTokens();
    int index = 0;

    std::optional<Error> errorResult = std::nullopt;
    while (index < tokens.size())
    {
        if (tokens[index].GetToken() == Token::SELECT)
            errorResult = CheckSelectStatement(tokens, index);
        else if (tokens[index].GetToken() == Token::INSERT)
            errorResult = CheckInsertStatement(tokens, index);
        else if (tokens[index].GetToken() == Token::UPDATE)
            errorResult = CheckUpdateStatement(tokens, index);
        else if (tokens[index].GetToken() == Token::DELETE)
            errorResult = CheckDeleteStatement(tokens, index);
        else if (tokens[index].GetToken() == Token::ALTER)
            errorResult = CheckAlterCommand(tokens, index);
        else if (index + 1 < tokens.size())
        {
            if (tokens[index].GetToken() == Token::CREATE && tokens[index + 1].GetToken() == Token::DATABASE)
                errorResult = CheckCreateDatabaseStatement(tokens, index);
            else if (tokens[index].GetToken() == Token::DROP && tokens[index + 1].GetToken() == Token::DATABASE)
                errorResult = CheckDropDatabaseStatement(tokens, index);
            else if (tokens[index].GetToken() == Token::CREATE && tokens[index + 1].GetToken() == Token::TABLE)
                errorResult = CheckCreateTableStatement(tokens, index);
            else if (tokens[index].GetToken() == Token::DROP && tokens[index + 1].GetToken() == Token::TABLE)
                errorResult = CheckDropTableStatement(tokens, index);
        }

        if (errorResult != std::nullopt)
            return errorResult;
    }

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckInsertStatement(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    p_symbolTable->AddNode(NonTerminalToken::INSERT);

    errorResult = Consume(tokens, Token::INSERT, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::INTO, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::VALUES, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::LEFT_PARENTHESIS, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = CheckInsertValues(tokens, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::RIGHT_PARENTHESIS, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::SEMICOLON, index);
    if (errorResult.has_value())
        return errorResult;

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckInsertValues(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    p_symbolTable->AddNode(NonTerminalToken::INSERT_VALUES);
    do
    {
        mustRepeat = false;

        errorResult = CheckInsertValue(tokens, index);
        if (errorResult.has_value())
            return errorResult;

        if (tokens[index].GetToken() == Token::COMMA)
        {
            errorResult = Consume(tokens, Token::COMMA, index);
            if (errorResult.has_value())
                return errorResult;

            mustRepeat = true;
        }
    } while (mustRepeat);

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckInsertValue(const std::vector<TokenDefinition> &tokens, int &index) const
{
    switch (tokens[index].GetToken())
    {
    case Token::VARIABLE:
    case Token::STRING:
    case Token::INTEGER_NUMBER:
    case Token::DECIMAL_NUMBER:
    case Token::NULL_VALUE:
        return Consume(tokens, tokens[index].GetToken(), index);
    default:
        return Error(ErrorType::InvalidToken, "It was expected a valid insertable value");
    }
}

std::optional<Error> SyntaxAnalyzer::CheckInsertIdentifiers(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    p_symbolTable->AddNode(NonTerminalToken::INSERT_IDENTIFIERS);
    do
    {
        mustRepeat = false;

        errorResult = Consume(tokens, Token::IDENTIFIER, index);
        if (errorResult.has_value())
            return errorResult;

        if (tokens[index].GetToken() == Token::COMMA)
        {
            errorResult = Consume(tokens, Token::COMMA, index);
            if (errorResult.has_value())
                return errorResult;

            mustRepeat = true;
        }
    } while (mustRepeat);

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckUpdateStatement(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    p_symbolTable->AddNode(NonTerminalToken::UPDATE);

    errorResult = Consume(tokens, Token::UPDATE, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::SET, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = CheckUpdateSet(tokens, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::WHERE, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = CheckLogicalExpression(tokens, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::SEMICOLON, index);
    if (errorResult.has_value())
        return errorResult;

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckUpdateSet(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    p_symbolTable->AddNode(NonTerminalToken::UPDATE_SET);

    do
    {
        mustRepeat = false;

        errorResult = CheckIdentifierAttribute(tokens, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(tokens, Token::EQUAL, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = CheckFactorExpression(tokens, index);
        if (errorResult.has_value())
            return errorResult;

        if (tokens[index].GetToken() == Token::COMMA)
        {
            errorResult = Consume(tokens, Token::COMMA, index);
            if (errorResult.has_value())
                return errorResult;

            mustRepeat = true;
        }
    } while (mustRepeat);

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckDeleteStatement(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    p_symbolTable->AddNode(NonTerminalToken::DELETE);

    errorResult = Consume(tokens, Token::DELETE, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::WHERE, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = CheckLogicalExpression(tokens, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::SEMICOLON, index);
    if (errorResult.has_value())
        return errorResult;

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckCreateDatabaseStatement(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    p_symbolTable->AddNode(NonTerminalToken::CREATE_DATABASE);

    errorResult = Consume(tokens, Token::CREATE, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::DATABASE, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::SEMICOLON, index);
    if (errorResult.has_value())
        return errorResult;

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckDropDatabaseStatement(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    p_symbolTable->AddNode(NonTerminalToken::DROP_DATABASE);

    errorResult = Consume(tokens, Token::DROP, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::DATABASE, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::SEMICOLON, index);
    if (errorResult.has_value())
        return errorResult;

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckCreateTableStatement(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    p_symbolTable->AddNode(NonTerminalToken::CREATE_TABLE);

    errorResult = Consume(tokens, Token::CREATE, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::TABLE, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::LEFT_PARENTHESIS, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = CheckColumnsDefinitions(tokens, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::RIGHT_PARENTHESIS, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::SEMICOLON, index);
    if (errorResult.has_value())
        return errorResult;

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckColumnsDefinitions(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    p_symbolTable->AddNode(NonTerminalToken::COLUMNS_DEFINITIONS);

    do
    {
        mustRepeat = false;

        errorResult = Consume(tokens, Token::IDENTIFIER, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = CheckColumnDefinition(tokens, index);
        if (errorResult.has_value())
            return errorResult;

        if (tokens[index].GetToken() == Token::COMMA)
        {
            errorResult = Consume(tokens, Token::COMMA, index);
            if (errorResult.has_value())
                return errorResult;

            mustRepeat = true;
        }
    } while (mustRepeat);

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckColumnDefinition(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    p_symbolTable->AddNode(NonTerminalToken::COLUMN_DEFINITION);

    if (IsColumnConstraint(tokens, index))
        CheckColumnContraints(tokens, index);

    CheckTextType(tokens, index);

    if (IsColumnConstraint(tokens, index))
        CheckColumnContraints(tokens, index);

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckColumnType(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    p_symbolTable->AddNode(NonTerminalToken::COLUMN_TYPE);

    Token currentToken = tokens[index].GetToken();
    if (currentToken == Token::INTEGER_NUMBER || currentToken == Token::DECIMAL_NUMBER)
    {
        errorResult = Consume(tokens, tokens[index].GetToken(), index);
        if (errorResult.has_value())
            return errorResult;
    }
    else
    {
        errorResult = CheckTextType(tokens, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(tokens, Token::LEFT_PARENTHESIS, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(tokens, Token::INTEGER_NUMBER, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(tokens, Token::RIGHT_PARENTHESIS, index);
        if (errorResult.has_value())
            return errorResult;
    }

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckTextType(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    p_symbolTable->AddNode(NonTerminalToken::TEXT_TYPE);

    switch (tokens[index].GetToken())
    {
    case Token::NVARCHAR:
    case Token::VARCHAR:
    case Token::CHAR:
    case Token::NCHAR:
        return Consume(tokens, tokens[index].GetToken(), index);
    default:
        return Error(ErrorType::InvalidToken, "It was expected a valid insertable value");
    }

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckColumnContraints(const std::vector<TokenDefinition> &tokens, int &index) const
{
    p_symbolTable->AddNode(NonTerminalToken::COLUMN_CONSTRAINTS);

    do
    {
        auto errorResult = CheckColumnContraint(tokens, index);
        if (errorResult.has_value())
            return errorResult;
    } while (IsColumnConstraint(tokens, index));

    p_symbolTable->TierUp();

    return std::nullopt;
}

bool SyntaxAnalyzer::IsColumnConstraint(const std::vector<TokenDefinition> &tokens, const int &index) const
{
    Token currentToken = tokens[index].GetToken();
    return currentToken == Token::PRIMARY ||
           currentToken == Token::NULL_VALUE ||
           currentToken == Token::NOT ||
           currentToken == Token::UNIQUE;
}

std::optional<Error> SyntaxAnalyzer::CheckColumnContraint(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    p_symbolTable->AddNode(NonTerminalToken::COLUMN_CONSTRAINT);

    if (tokens[index].GetToken() == Token::PRIMARY)
    {
        errorResult = Consume(tokens, Token::PRIMARY, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(tokens, Token::KEY, index);
        if (errorResult.has_value())
            return errorResult;
    }
    else if (tokens[index].GetToken() == Token::NOT)
    {
        errorResult = Consume(tokens, Token::NOT, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(tokens, Token::NULL_VALUE, index);
        if (errorResult.has_value())
            return errorResult;
    }
    else if (tokens[index].GetToken() == Token::UNIQUE || tokens[index].GetToken() == Token::NULL_VALUE)
    {
        errorResult = Consume(tokens, tokens[index].GetToken(), index);
        if (errorResult.has_value())
            return errorResult;
    }
    else
        return Error(ErrorType::InvalidToken, "It was expected a valid insertable value");

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckAlterTableStatement(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    p_symbolTable->AddNode(NonTerminalToken::ALTER_TABLE);

    errorResult = Consume(tokens, Token::ALTER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::TABLE, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = CheckAlterCommand(tokens, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::SEMICOLON, index);
    if (errorResult.has_value())
        return errorResult;

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckAlterCommand(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    p_symbolTable->AddNode(NonTerminalToken::ALTER_COMMAND);

    if (tokens[index].GetToken() == Token::ADD)
    {
        errorResult = Consume(tokens, Token::ADD, index);
        if (errorResult.has_value())
            return errorResult;

        if (tokens[index].GetToken() == Token::FOREIGN)
        {
            errorResult = ConsumeAddForeignKey(tokens, index);
            if (errorResult.has_value())
                return errorResult;
        }
        else
        {
            errorResult = Consume(tokens, Token::COLUMN, index);
            if (errorResult.has_value())
                return errorResult;

            errorResult = CheckColumnDefinition(tokens, index);
            if (errorResult.has_value())
                return errorResult;
        }
    }
    else if (tokens[index].GetToken() == Token::DROP)
    {
        errorResult = Consume(tokens, Token::DROP, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(tokens, Token::COLUMN, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(tokens, Token::IDENTIFIER, index);
        if (errorResult.has_value())
            return errorResult;
    }
    else
        return Error(ErrorType::InvalidToken, "It was expected a valid insertable value");

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::ConsumeAddForeignKey(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;
    errorResult = Consume(tokens, Token::FOREIGN, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::KEY, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::REFERENCES, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::LEFT_PARENTHESIS, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::RIGHT_PARENTHESIS, index);
    if (errorResult.has_value())
        return errorResult;

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckDropTableStatement(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;

    p_symbolTable->AddNode(NonTerminalToken::DROP_TABLE);

    errorResult = Consume(tokens, Token::DROP, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::TABLE, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::SEMICOLON, index);
    if (errorResult.has_value())
        return errorResult;

    p_symbolTable->TierUp();

    return std::nullopt;
}