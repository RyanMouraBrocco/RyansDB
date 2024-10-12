#include "CreateTableProcessor.hpp"

std::optional<Error> CreateTableProcessor::CreateTableExecution(std::string databaseName, std::shared_ptr<ParserTreeNode> queryTree)
{
    auto identifierTokenValue = queryTree->GetChildren()[0]->GetToken().GetValue();
    if (!std::holds_alternative<TokenDefinition>(identifierTokenValue))
        return Error(ErrorType::Unexpected, "It was expected a identifier but a NonTerminalToken was found");

    auto identifier = std::get<TokenDefinition>(identifierTokenValue);
    if (identifier.GetToken() != Token::IDENTIFIER)
        return Error(ErrorType::Unexpected, "It was expected a identifier but another token was found");

    auto tableName = identifier.GetUpperCaseLexeme();

    // check if table already exists

    auto columnDefinitions = queryTree->GetChildren()[1];
    auto columnsResult = ExtractColumns(columnDefinitions);
    if (std::holds_alternative<Error>(columnsResult))
        return std::get<Error>(columnsResult);

    auto columns = std::get<std::vector<TableColumnDefinition>>(columnsResult);

    return m_dataAccess.CreateTableInDatabaseFile(databaseName, tableName, columns);
}

std::variant<std::vector<TableColumnDefinition>, Error> CreateTableProcessor::ExtractColumns(std::shared_ptr<ParserTreeNode> columnDefinitions)
{
    auto columnDefinitionsItems = columnDefinitions->GetChildren();
    std::vector<TableColumnDefinition> tableColumns;
    tableColumns.reserve((int)columnDefinitionsItems.size() / 2);

    for (int i = 0; i < columnDefinitionsItems.size(); i += 2)
    {
        auto columnNameResult = ExtractColumnName(columnDefinitionsItems[i]);
        if (std::holds_alternative<Error>(columnNameResult))
            return std::get<Error>(columnNameResult);

        auto columnDefinitionTree = columnDefinitionsItems[i + 1];
        auto columnDefintionToken = columnDefinitionTree->GetToken().GetValue();
        if (!std::holds_alternative<NonTerminalToken>(columnDefintionToken) && std::get<NonTerminalToken>(columnDefintionToken) != NonTerminalToken::COLUMN_DEFINITION)
            return Error(ErrorType::Unexpected, "");

        if (columnDefinitionTree->GetChildren().size() == 0)
            return Error(ErrorType::Unexpected, "");

        auto leftColumnDefinitionTypeTree = columnDefinitionTree->GetChildren()[0];
        auto leftColumnDefinitionToken = leftColumnDefinitionTypeTree->GetToken().GetValue();
        if (!std::holds_alternative<NonTerminalToken>(leftColumnDefinitionToken))
            return Error(ErrorType::Unexpected, "");

        std::tuple<bool, bool> contraints;
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
                auto constraintsResult = ExtractColumnConstraints(columnConstraintsTree);
                if (std::holds_alternative<Error>(constraintsResult))
                    return std::get<Error>(constraintsResult);

                contraints = std::get<std::tuple<bool, bool>>(constraintsResult);
            }
        }
        else
        {
            auto constraintsResult = ExtractColumnConstraints(leftColumnDefinitionTypeTree);
            if (std::holds_alternative<Error>(constraintsResult))
                return std::get<Error>(constraintsResult);

            contraints = std::get<std::tuple<bool, bool>>(constraintsResult);

            auto columnTypeTree = columnDefinitionTree->GetChildren()[1];
            auto columnsResult = ExtractColumnType(columnTypeTree);
            if (std::holds_alternative<Error>(columnsResult))
                return std::get<Error>(columnsResult);

            columnNameAndType = &std::get<std::tuple<TokenDefinition, int>>(columnsResult);
        }

        auto tableColumnDefinition = TableColumnDefinition(std::get<std::string>(columnNameResult), std::get<0>(*columnNameAndType), std::get<1>(*columnNameAndType));
        tableColumnDefinition.notNull = std::get<0>(contraints);
        tableColumnDefinition.primaryKey = std::get<1>(contraints);
        tableColumns.push_back(tableColumnDefinition);
    }

    return tableColumns;
}

std::variant<std::string, Error> CreateTableProcessor::ExtractColumnName(std::shared_ptr<ParserTreeNode> columnNameTree)
{
    auto columnNameDefinition = columnNameTree->GetToken().GetValue();
    if (!std::holds_alternative<TokenDefinition>(columnNameDefinition))
        return Error(ErrorType::Unexpected, "It was expected a TokenDefintion but a NonTerminalToken was found");

    auto columnName = std::get<TokenDefinition>(columnNameDefinition);
    if (columnName.GetToken() != Token::IDENTIFIER)
        return Error(ErrorType::Unexpected, "It was expected a identifier but another token was found");

    return columnName.GetUpperCaseLexeme();
}

std::variant<std::tuple<TokenDefinition, int>, Error> CreateTableProcessor::ExtractColumnType(std::shared_ptr<ParserTreeNode> columnTypeTree)
{
    auto columnTypeToken = columnTypeTree->GetToken().GetValue();
    if (!std::holds_alternative<NonTerminalToken>(columnTypeToken) || std::get<NonTerminalToken>(columnTypeToken) != NonTerminalToken::COLUMN_TYPE)
        return Error(ErrorType::Unexpected, "It was expected a COLUMN_TYPE NonTerminalToken but it was found somthing else");

    auto typeTree = columnTypeTree->GetChildren()[0];
    auto typeTokenType = typeTree->GetToken().GetValue();
    if (std::holds_alternative<NonTerminalToken>(typeTokenType))
    {
        if (std::get<NonTerminalToken>(typeTokenType) != NonTerminalToken::TEXT_TYPE)
            return Error(ErrorType::Unexpected, "It was expected a TEXT_TYPE NonTerminalToken but it was found somthing else");

        auto textTokenType = std::get<TokenDefinition>(typeTree->GetChildren()[0]->GetToken().GetValue());
        auto textTokenLength = stoi(std::get<TokenDefinition>(columnTypeTree->GetChildren()[1]->GetToken().GetValue()).GetUpperCaseLexeme());

        return std::tuple<TokenDefinition, int>(textTokenType, textTokenLength);
    }
    else
    {
        return std::tuple<TokenDefinition, int>(std::get<TokenDefinition>(typeTokenType), -1);
    }
}

std::variant<std::tuple<bool, bool>, Error> CreateTableProcessor::ExtractColumnConstraints(std::shared_ptr<ParserTreeNode> columnConstraintsTree)
{
    auto columnConstraintsTokenType = columnConstraintsTree->GetToken().GetValue();
    if (std::holds_alternative<NonTerminalToken>(columnConstraintsTokenType) && std::get<NonTerminalToken>(columnConstraintsTokenType) != NonTerminalToken::COLUMN_CONSTRAINTS)
        return Error(ErrorType::Unexpected, "It was expected a COLUMN_CONTRAINTS NonTerminalToken but it was found somthing else");

    auto contraintsItems = columnConstraintsTree->GetChildren();
    std::tuple<bool, bool> contraints = std::make_tuple(false, false);
    for (int i = 0; i < contraintsItems.size(); i++)
    {
        auto constraintResult = contraintsItems[i]->GetToken().GetValue();
        if (!std::holds_alternative<NonTerminalToken>(constraintResult) || std::get<NonTerminalToken>(constraintResult) != NonTerminalToken::COLUMN_CONSTRAINT)
            return Error(ErrorType::Unexpected, "It was expected a COLUMN_CONTRAINT NonTerminalToken but it was found somthing else");

        auto extractColumnConstraintResult = ExtractColumnConstraint(contraintsItems[i], contraints);
        if (extractColumnConstraintResult.has_value())
            return extractColumnConstraintResult.value();
    }

    return contraints;
}

std::optional<Error> CreateTableProcessor::ExtractColumnConstraint(std::shared_ptr<ParserTreeNode> columnConstraintTree, std::tuple<bool, bool> &constraintsTuple)
{
    auto firstConstraintToken = columnConstraintTree->GetChildren()[0]->GetToken().GetValue();
    if (!std::holds_alternative<TokenDefinition>(firstConstraintToken))
        return Error(ErrorType::Unexpected, "It was expected a NonTerminalToken but a TokenDefinition was found");

    auto fisrtConstraintToken = std::get<TokenDefinition>(firstConstraintToken);

    if (fisrtConstraintToken.GetToken() == Token::PRIMARY)
    {
        if (columnConstraintTree->GetChildren().size() != 2 ||
            !std::holds_alternative<TokenDefinition>(columnConstraintTree->GetChildren()[1]->GetToken().GetValue()) ||
            std::get<TokenDefinition>(columnConstraintTree->GetChildren()[1]->GetToken().GetValue()).GetToken() != Token::KEY)
            return Error(ErrorType::Unexpected, "It was expected a PRIMARY constraint with more token definitions");

        std::get<1>(constraintsTuple) = true;
    }
    else if (fisrtConstraintToken.GetToken() == Token::NOT)
    {
        if (columnConstraintTree->GetChildren().size() != 2 ||
            !std::holds_alternative<TokenDefinition>(columnConstraintTree->GetChildren()[1]->GetToken().GetValue()) ||
            std::get<TokenDefinition>(columnConstraintTree->GetChildren()[1]->GetToken().GetValue()).GetToken() != Token::NULL_VALUE)
            return Error(ErrorType::Unexpected, "It was expected a NOT constraint with more token definitions");

        std::get<0>(constraintsTuple) = true;
    }
    else if (fisrtConstraintToken.GetToken() == Token::NULL_VALUE)
    {
        if (columnConstraintTree->GetChildren().size() != 1)
            return Error(ErrorType::Unexpected, "It was expected a NULL constraint with only one token definitions");

        std::get<0>(constraintsTuple) = false;
    }
    else
        return Error(ErrorType::Unexpected, "It was expected a valid constraint");

    return std::nullopt;
}

std::shared_ptr<CreateTableProcessor> CreateTableProcessor::p_singleton = nullptr;
std::mutex CreateTableProcessor::m_mutex;

std::shared_ptr<CreateTableProcessor> CreateTableProcessor::GetInstance()
{
    if (p_singleton == nullptr)
    {
        m_mutex.lock();
        if (p_singleton == nullptr)
        {
            p_singleton = std::make_shared<CreateTableProcessor>();
        }
        m_mutex.unlock();
    }

    return p_singleton;
}