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
    auto columns = ExtractColumns(columnDefinitions);

    return Error(ErrorType::Unexpected, "");

    // return m_dataAccess.CreateTableInDatabaseFile(databaseName, );
}

std::variant<std::vector<TableColumnDefinition>, Error> ExecuteEngine::ExtractColumns(std::shared_ptr<ParserTreeNode> columnDefinitions)
{
    std::vector<TableColumnDefinition> tableColumns;
    auto columnDefinitionHelper = columnDefinitions;
    bool hasMoreDefinition = true;
    while (hasMoreDefinition)
    {
        auto columnNameDefinition = columnDefinitionHelper->GetChildren()[0]->GetToken().GetValue();
        if (!std::holds_alternative<TokenDefinition>(columnNameDefinition))
            return Error(ErrorType::Unexpected, "");

        auto columnName = std::get<TokenDefinition>(columnNameDefinition);
        if (columnName.GetToken() != Token::IDENTIFIER)
            return Error(ErrorType::Unexpected, "");

        auto columnDefinitionTree = columnDefinitionHelper->GetChildren()[1];
        auto columnDefintionToken = columnDefinitionTree->GetToken().GetValue();
        if (!std::holds_alternative<NonTerminalToken>(columnDefintionToken) && std::get<NonTerminalToken>(columnDefintionToken) != NonTerminalToken::COLUMN_DEFINITION)
            return Error(ErrorType::Unexpected, "");

        if (columnDefinitionTree->GetChildren().size() == 0)
            return Error(ErrorType::Unexpected, "");

        auto leftColumnDefinitionTypeTree = columnDefinitionTree->GetChildren()[0];
        auto leftColumnDefinitionToken = leftColumnDefinitionTypeTree->GetToken().GetValue();
        if (!std::holds_alternative<NonTerminalToken>(leftColumnDefinitionToken))
            return Error(ErrorType::Unexpected, "");

        std::vector<TokenDefinition> contraints;
        std::tuple<TokenDefinition, int> *columnNameAndType = nullptr;
        if (std::get<NonTerminalToken>(leftColumnDefinitionToken) == NonTerminalToken::COLUMN_TYPE)
        {
            auto columnsResult = ExtractColumnType(leftColumnDefinitionTypeTree);
            if (std::holds_alternative<Error>(columnsResult))
                return std::get<Error>(columnsResult);

            columnNameAndType = &std::get<std::tuple<TokenDefinition, int>>(columnsResult);

            if (columnDefinitionTree->GetChildren().size() > 1)
            {
                auto columnConstraintsTree = columnDefinitionTree->GetChildren()[1];
                auto columnConstraintsTokenType = columnConstraintsTree->GetToken().GetValue();
                if (std::holds_alternative<NonTerminalToken>(columnConstraintsTokenType) && std::get<NonTerminalToken>(columnConstraintsTokenType) != NonTerminalToken::COLUMN_CONSTRAINTS)
                    return Error(ErrorType::Unexpected, "");

                auto constraintsResult = ExtractColumnConstraints(columnConstraintsTree);
                if (std::holds_alternative<Error>(constraintsResult))
                    return std::get<Error>(constraintsResult);

                auto contraints = std::get<std::vector<TokenDefinition>>(constraintsResult);
            }
        }
        else
        {
            auto constraintsResult = ExtractColumnConstraints(leftColumnDefinitionTypeTree);
            if (std::holds_alternative<Error>(constraintsResult))
                return std::get<Error>(constraintsResult);

            auto contraints = std::get<std::vector<TokenDefinition>>(constraintsResult);

            auto columnTypeTree = columnDefinitionTree->GetChildren()[1];
            auto columnTypeToken = columnTypeTree->GetToken().GetValue();
            if (std::holds_alternative<NonTerminalToken>(columnTypeToken) && std::get<NonTerminalToken>(columnTypeToken) != NonTerminalToken::COLUMN_TYPE)
                return Error(ErrorType::Unexpected, "");

            auto columnsResult = ExtractColumnType(columnTypeTree);
            if (std::holds_alternative<Error>(columnsResult))
                return std::get<Error>(columnsResult);

            columnNameAndType = &std::get<std::tuple<TokenDefinition, int>>(columnsResult);
        }

        auto tableColumnDefinition = TableColumnDefinition(columnName.GetUpperCaseLexeme(), std::get<0>(*columnNameAndType), std::get<1>(*columnNameAndType));
        tableColumnDefinition.constraints = contraints;
        tableColumns.push_back(tableColumnDefinition);

        if (columnDefinitionHelper->GetChildren().size() > 2)
        {
            auto morecolumnDefinition = columnDefinitionHelper->GetChildren()[2];
            if (!std::holds_alternative<NonTerminalToken>(morecolumnDefinition->GetToken().GetValue()) || std::get<NonTerminalToken>(morecolumnDefinition->GetToken().GetValue()) != NonTerminalToken::COLUMNS_DEFINITIONS)
                return Error(ErrorType::Unexpected, "");

            columnDefinitionHelper = morecolumnDefinition;
        }
        else
            hasMoreDefinition = false;
    }

    return tableColumns;
}

std::variant<std::tuple<TokenDefinition, int>, Error> ExecuteEngine::ExtractColumnType(std::shared_ptr<ParserTreeNode> columnTypeTree)
{
    auto typeTree = columnTypeTree->GetChildren()[0];
    auto typeTokenType = typeTree->GetToken().GetValue();
    if (std::holds_alternative<NonTerminalToken>(typeTokenType))
    {
        if (std::get<NonTerminalToken>(typeTokenType) != NonTerminalToken::TEXT_TYPE)
            return Error(ErrorType::Unexpected, "");

        auto textTokenType = std::get<TokenDefinition>(typeTree->GetChildren()[0]->GetToken().GetValue());
        auto textTokenLength = stoi(std::get<TokenDefinition>(typeTree->GetChildren()[1]->GetToken().GetValue()).GetUpperCaseLexeme());

        return std::tuple<TokenDefinition, int>(textTokenType, textTokenLength);
    }
    else
    {
        return std::tuple<TokenDefinition, int>(std::get<TokenDefinition>(typeTree->GetToken().GetValue()), 0);
    }
}

std::variant<std::vector<TokenDefinition>, Error> ExecuteEngine::ExtractColumnConstraints(std::shared_ptr<ParserTreeNode> columnConstrainsTree)
{
    bool hasMoreContraints = true;
    auto contraintsTree = columnConstrainsTree;
    std::vector<TokenDefinition> contraints;
    while (hasMoreContraints)
    {
        auto constraintResult = contraintsTree->GetChildren()[0]->GetToken().GetValue();
        if (!std::holds_alternative<TokenDefinition>(constraintResult))
            return Error(ErrorType::Unexpected, "");

        contraints.push_back(std::get<TokenDefinition>(constraintResult));

        if (contraintsTree->GetChildren().size() > 1)
        {
            auto moreContraintsTree = contraintsTree->GetChildren()[1];
            if (!std::holds_alternative<NonTerminalToken>(moreContraintsTree->GetToken().GetValue()) || std::get<NonTerminalToken>(moreContraintsTree->GetToken().GetValue()) != NonTerminalToken::COLUMN_CONSTRAINTS)
                return Error(ErrorType::Unexpected, "");

            contraintsTree = moreContraintsTree;
        }
        else
            hasMoreContraints = false;
    }

    return contraints;
}