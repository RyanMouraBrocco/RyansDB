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
        return p_databaseProcessor->CreateDatabaseExecution(queryTree);
    case NonTerminalToken::DROP_DATABASE:
        return p_databaseProcessor->DropDatabaseExecution(queryTree);
    case NonTerminalToken::CREATE_TABLE:
        return p_tableProcessor->CreateTableExecution("HOUSE", queryTree);
    default:
        return Error(ErrorType::Unexpected, "A invalid non terminal token was sent");
    }
}