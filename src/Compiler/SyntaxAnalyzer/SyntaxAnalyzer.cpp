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

std::optional<Error> SyntaxAnalyzer::Consume(const std::vector<TokenDefinition> &tokens, const Token &&expextedToken, int &index) const
{
    if (index >= tokens.size())
        return Error(ErrorType::InvalidToken, "The command finished but it was expected a " + tokens[index].GetUpperCaseLexeme());

    if (tokens[index].GetToken() != expextedToken)
        return Error(ErrorType::InvalidToken, "It was expected a <HERE> but was found a " + tokens[index].GetUpperCaseLexeme());

    p_symbolTable->AddNode(tokens[index]);

    index++;

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckSelectStatement(const std::vector<TokenDefinition> &tokens, int &index) const
{
    p_symbolTable->AddNode(NonTerminalToken::SELECT);

    std::optional<Error> errorResult = std::nullopt;
    errorResult = Consume(tokens, Token::SELECT, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = CheckSelectItems(tokens, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = Consume(tokens, Token::FROM, index);
    if (errorResult.has_value())
        return errorResult;

    errorResult = CheckFrom(tokens, index);
    if (errorResult.has_value())
        return errorResult;

    if (tokens[index].GetToken() == Token::WHERE)
    {
        errorResult = Consume(tokens, Token::WHERE, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = CheckLogicalExpression(tokens, index);
        if (errorResult.has_value())
            return errorResult;
    }

    errorResult = Consume(tokens, Token::SEMICOLON, index);
    if (errorResult.has_value())
        return errorResult;

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckSelectItems(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;

    p_symbolTable->AddNode(NonTerminalToken::SELECT_ITEM);
    do
    {
        mustRepeat = false;
        if (tokens[index].GetToken() == Token::ASTERISK)
        {
            errorResult = Consume(tokens, Token::ASTERISK, index);
            if (errorResult.has_value())
                return errorResult;
        }
        else
        {
            errorResult = Consume(tokens, Token::IDENTIFIER, index);
            if (errorResult.has_value())
                return errorResult;
        }

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

std::optional<Error> SyntaxAnalyzer::CheckFrom(const std::vector<TokenDefinition> &tokens, int &index) const
{
    p_symbolTable->AddNode(NonTerminalToken::FROM);

    std::optional<Error> errorResult = CheckTableName(tokens, index);
    if (errorResult.has_value())
        return errorResult;

    if (tokens[index].GetToken() == Token::INNER)
    {
        std::optional<Error> errorResult = CheckJoin(tokens, index);
        if (errorResult.has_value())
            return errorResult;
    }

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckJoin(const std::vector<TokenDefinition> &tokens, int &index) const
{
    std::optional<Error> errorResult = std::nullopt;
    int depthNonTerminalToken = 0;

    do
    {
        p_symbolTable->AddNode(NonTerminalToken::JOIN);
        depthNonTerminalToken++;

        errorResult = Consume(tokens, Token::INNER, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(tokens, Token::JOIN, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = CheckTableName(tokens, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(tokens, Token::ON, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = CheckLogicalExpression(tokens, index);
        if (errorResult.has_value())
            return errorResult;

    } while (tokens[index].GetToken() == Token::INNER);

    for (int i = 0; i < depthNonTerminalToken; i++)
        p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckTableName(const std::vector<TokenDefinition> &tokens, int &index) const
{
    p_symbolTable->AddNode(NonTerminalToken::TABLE_NAME);

    std::optional<Error> errorResult = Consume(tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    if (tokens[index].GetToken() == Token::IDENTIFIER)
    {
        errorResult = Consume(tokens, Token::IDENTIFIER, index);
        if (errorResult.has_value())
            return errorResult;
    }

    p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckLogicalExpression(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;
    int depthNonTerminalToken = 0;

    do
    {
        p_symbolTable->AddNode(NonTerminalToken::LOGICAL_EXPRESSION);
        depthNonTerminalToken++;
        mustRepeat = false;

        errorResult = CheckAndOperation(tokens, index);
        if (errorResult.has_value())
            return errorResult;

        if (tokens[index].GetToken() == Token::OR)
        {
            errorResult = Consume(tokens, Token::OR, index);
            if (errorResult.has_value())
                return errorResult;

            mustRepeat = true;
        }
    } while (mustRepeat);

    for (int i = 0; i < depthNonTerminalToken; i++)
        p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckAndOperation(const std::vector<TokenDefinition> &tokens, int &index) const
{
    bool mustRepeat;
    std::optional<Error> errorResult = std::nullopt;
    int depthNonTerminalToken = 0;

    do
    {
        p_symbolTable->AddNode(NonTerminalToken::AND_EXPRESSION);
        depthNonTerminalToken++;
        mustRepeat = false;

        errorResult = CheckTokenExpression(tokens, index);
        if (errorResult.has_value())
            return errorResult;

        if (tokens[index].GetToken() == Token::AND)
        {
            errorResult = Consume(tokens, Token::AND, index);
            if (errorResult.has_value())
                return errorResult;

            mustRepeat = true;
        }
    } while (mustRepeat);

    for (int i = 0; i < depthNonTerminalToken; i++)
        p_symbolTable->TierUp();

    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckTokenExpression(const std::vector<TokenDefinition> &tokens, int &index) const
{
    p_symbolTable->AddNode(NonTerminalToken::TOKEN_EXPRESSION);

    std::optional<Error> errorResult = std::nullopt;
    if (tokens[index].GetToken() == Token::NOT)
    {
        errorResult = Consume(tokens, Token::NOT, index);
        if (errorResult.has_value())
            return errorResult;

        if (tokens[index].GetToken() == Token::NULL_VALUE)
        {
            errorResult = Consume(tokens, Token::NULL_VALUE, index);
            if (errorResult.has_value())
                return errorResult;
        }

        errorResult = CheckFactorExpression(tokens, index);
        if (errorResult.has_value())
            return errorResult;
    }
    else if (tokens[index].GetToken() == Token::LEFT_PARENTHESIS)
    {
        errorResult = Consume(tokens, Token::LEFT_PARENTHESIS, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = CheckLogicalExpression(tokens, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(tokens, Token::RIGHT_PARENTHESIS, index);
        if (errorResult.has_value())
            return errorResult;
    }
    else
    {
        errorResult = CheckComparisonExpression(tokens, index);
        if (errorResult.has_value())
            return errorResult;
    }

    p_symbolTable->TierUp();
    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckComparisonExpression(const std::vector<TokenDefinition> &tokens, int &index) const
{
    p_symbolTable->AddNode(NonTerminalToken::COMPARISON_EXPRESSION);

    std::optional<Error> errorResult = std::nullopt;
    errorResult = CheckFactorExpression(tokens, index);
    if (errorResult.has_value())
        return errorResult;

    if (SymbolTable::IsComparisionToken(tokens[index].GetToken()))
    {
        errorResult = CheckCompareAction(tokens, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = CheckFactorExpression(tokens, index);
        if (errorResult.has_value())
            return errorResult;
    }

    p_symbolTable->TierUp();
    return std::nullopt;
}

std::optional<Error> SyntaxAnalyzer::CheckCompareAction(const std::vector<TokenDefinition> &tokens, int &index) const
{
    p_symbolTable->AddNode(NonTerminalToken::COMPARE_ACTION);
    std::optional<Error> errorResult = std::nullopt;

    if (SymbolTable::IsComparisionToken(tokens[index].GetToken()))
        errorResult = Consume(tokens, tokens[index].GetToken(), index);
    else
        errorResult = Error(ErrorType::InvalidToken, "It as expected a valid comparison token but receive a " + tokens[index].GetUpperCaseLexeme());

    p_symbolTable->TierUp();
    return errorResult;
}

std::optional<Error> SyntaxAnalyzer::CheckFactorExpression(const std::vector<TokenDefinition> &tokens, int &index) const
{
    p_symbolTable->AddNode(NonTerminalToken::FACTOR_EXPRESSION);
    std::optional<Error> errorResult = std::nullopt;

    if (tokens[index].GetToken() == Token::IDENTIFIER)
        errorResult = CheckIdentifierAttribute(tokens, index);
    else if (tokens[index].GetToken() == Token::NULL_VALUE)
        errorResult = Consume(tokens, Token::NULL_VALUE, index);
    else if (SymbolTable::IsFactorToken(tokens[index].GetToken()))
        errorResult = Consume(tokens, tokens[index].GetToken(), index);
    else
        errorResult = Error(ErrorType::InvalidToken, "It as expected a valid factor but receive a " + tokens[index].GetUpperCaseLexeme());

    p_symbolTable->TierUp();
    return errorResult;
}

std::optional<Error> SyntaxAnalyzer::CheckIdentifierAttribute(const std::vector<TokenDefinition> &tokens, int &index) const
{
    p_symbolTable->AddNode(NonTerminalToken::IDENTIFIER_ATTRIBUTE);
    std::optional<Error> errorResult = std::nullopt;
    errorResult = Consume(tokens, Token::IDENTIFIER, index);
    if (errorResult.has_value())
        return errorResult;

    if (tokens[index].GetToken() == Token::DOT)
    {
        errorResult = Consume(tokens, Token::DOT, index);
        if (errorResult.has_value())
            return errorResult;

        errorResult = Consume(tokens, Token::IDENTIFIER, index);
        if (errorResult.has_value())
            return errorResult;
    }

    p_symbolTable->TierUp();

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