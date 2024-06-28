#pragma once

#include <memory>
#include <optional>
#include "../SymbolTable/SymbolTable.hpp"

class SyntaxAnalyzer
{
private:
    std::shared_ptr<SymbolTable> p_symbolTable;

    std::optional<Error> Consume(const std::vector<TokenDefinition> &tokens, const Token &&expextedToken, int &index) const;
    std::optional<Error> CheckSelectStatement(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckSelectItems(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckFrom(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckTableName(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckJoin(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckLogicalExpression(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckAndOperation(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckTokenExpression(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckComparisonExpression(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckCompareAction(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckFactorExpression(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckIdentifierAttribute(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckInsertStatement(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckInsertValues(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckInsertValue(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckInsertIdentifiers(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckUpdateStatement(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckUpdateSet(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckDeleteStatement(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckCreateDatabaseStatement(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckDropDatabaseStatement(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckCreateTableStatement(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckColumnsDefinitions(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckColumnDefinition(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckColumnType(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckTextType(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckColumnContraints(const std::vector<TokenDefinition> &tokens, int &index) const;
    bool IsColumnConstraint(const std::vector<TokenDefinition> &tokens, const int &index) const;
    std::optional<Error> CheckColumnContraint(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckAlterTableStatement(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckAlterCommand(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> ConsumeAddForeignKey(const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckDropTableStatement(const std::vector<TokenDefinition> &tokens, int &index) const;

public:
    SyntaxAnalyzer(std::shared_ptr<SymbolTable> symbolTable);
    std::optional<Error> Execute();
};