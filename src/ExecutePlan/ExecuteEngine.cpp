#include "ExecuteEngine.hpp"

ExecuteEngine::ExecuteEngine()
{
}

std::optional<Error> ExecuteEngine::Execute(std::shared_ptr<ParserTreeNode> queryTree)
{
    auto token = queryTree->GetToken().GetValue();
    if (std::holds_alternative<TokenDefinition>(token))
        return Error(ErrorType::Unexpected, "Unexpected error when starts the execution engine");

    // i must call binder here first

    auto command = std::get<NonTerminalToken>(token);
    switch (command)
    {
    case NonTerminalToken::CREATE_DATABASE:
        return CreateDatabaseExecution(queryTree);
    case NonTerminalToken::DROP_DATABASE:
        return DropDatabaseExecution(queryTree);
    case NonTerminalToken::CREATE_TABLE:
        return CreateTableExecution("house", queryTree);
    default:
        return Error(ErrorType::Unexpected, "A invalid non terminal token was sent");
    }
}

std::optional<Error> ExecuteEngine::CreateDatabaseExecution(std::shared_ptr<ParserTreeNode> queryTree)
{
    auto identifierTokenValue = queryTree->GetChildren()[0]->GetToken().GetValue();
    if (std::holds_alternative<NonTerminalToken>(identifierTokenValue))
        return Error(ErrorType::Unexpected, "A invalid non terminal token was sent");

    auto identifier = std::get<TokenDefinition>(identifierTokenValue);
    if (identifier.GetToken() != Token::IDENTIFIER)
        return Error(ErrorType::Unexpected, "A invalid non terminal token was sent");

    return m_dataAccess.CreateDatabaseFile(identifier.GetUpperCaseLexeme());
}

std::optional<Error> ExecuteEngine::DropDatabaseExecution(std::shared_ptr<ParserTreeNode> queryTree)
{
    auto identifierTokenValue = queryTree->GetChildren()[0]->GetToken().GetValue();
    if (std::holds_alternative<NonTerminalToken>(identifierTokenValue))
        return Error(ErrorType::Unexpected, "A invalid non terminal token was sent");

    auto identifier = std::get<TokenDefinition>(identifierTokenValue);
    if (identifier.GetToken() != Token::IDENTIFIER)
        return Error(ErrorType::Unexpected, "A invalid non terminal token was sent");

    return m_dataAccess.DropDatabaseFile(identifier.GetUpperCaseLexeme());
}

std::optional<Error> ExecuteEngine::CreateTableExecution(std::string databaseName, std::shared_ptr<ParserTreeNode> queryTree)
{
    auto identifierTokenValue = queryTree->GetChildren()[0]->GetToken().GetValue();
    if (std::holds_alternative<NonTerminalToken>(identifierTokenValue))
        return Error(ErrorType::Unexpected, "A invalid non terminal token was sent");

    auto identifier = std::get<TokenDefinition>(identifierTokenValue);
    if (identifier.GetToken() != Token::IDENTIFIER)
        return Error(ErrorType::Unexpected, "A invalid non terminal token was sent");

    auto tableName = identifier.GetUpperCaseLexeme();

    // check if table already exists

    std::vector<TableColumnDefinition> tableColumns;

    auto columnDefinitions = queryTree->GetChildren()[1];
    while (true)
    {
        auto leftDefinition = columnDefinitions->GetChildren()[0];
        auto columnNameDefinition = leftDefinition->GetChildren()[0]->GetToken().GetValue();
        if (!std::holds_alternative<TokenDefinition>(columnNameDefinition))
            return Error(ErrorType::Unexpected, "");

        auto columnName = std::get<TokenDefinition>(columnNameDefinition);
        if (columnName.GetToken() != Token::IDENTIFIER)
            return Error(ErrorType::Unexpected, "");


    }

    return m_dataAccess.CreateTableInDatabaseFile(databaseName, );
}