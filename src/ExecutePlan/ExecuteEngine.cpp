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