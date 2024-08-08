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
    case NonTerminalToken::CREATE_TABLE:
        return CreateTableExecution();
        break;
    default:
        break;
    }
}

std::optional<Error> ExecuteEngine::CreateTableExecution()
{
}