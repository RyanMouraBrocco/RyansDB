#include "DatabaseProcessor.hpp"

std::optional<Error> DatabaseProcessor::CreateDatabaseExecution(std::shared_ptr<ParserTreeNode> queryTree)
{
    auto identifierTokenValue = queryTree->GetChildren()[0]->GetToken().GetValue();
    if (std::holds_alternative<NonTerminalToken>(identifierTokenValue))
        return Error(ErrorType::Unexpected, "A invalid non terminal token was sent");

    auto identifier = std::get<TokenDefinition>(identifierTokenValue);
    if (identifier.GetToken() != Token::IDENTIFIER)
        return Error(ErrorType::Unexpected, "A invalid non terminal token was sent");

    return m_dataAccess.CreateDatabaseFile(identifier.GetUpperCaseLexeme());
}

std::optional<Error> DatabaseProcessor::DropDatabaseExecution(std::shared_ptr<ParserTreeNode> queryTree)
{
    auto identifierTokenValue = queryTree->GetChildren()[0]->GetToken().GetValue();
    if (std::holds_alternative<NonTerminalToken>(identifierTokenValue))
        return Error(ErrorType::Unexpected, "A invalid non terminal token was sent");

    auto identifier = std::get<TokenDefinition>(identifierTokenValue);
    if (identifier.GetToken() != Token::IDENTIFIER)
        return Error(ErrorType::Unexpected, "A invalid non terminal token was sent");

    return m_dataAccess.DropDatabaseFile(identifier.GetUpperCaseLexeme());
}

std::shared_ptr<DatabaseProcessor> DatabaseProcessor::p_singleton = nullptr;
std::mutex DatabaseProcessor::m_mutex;

std::shared_ptr<DatabaseProcessor> DatabaseProcessor::GetInstance()
{
    if (p_singleton == nullptr)
    {
        m_mutex.lock();
        if (p_singleton == nullptr)
        {
            p_singleton = std::make_shared<DatabaseProcessor>();
        }
        m_mutex.unlock();
    }

    return p_singleton;
}