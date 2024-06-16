#pragma once

#include <memory>
#include <optional>
#include "../SymbolTable/SymbolTable.hpp"

class SyntaxAnalyzer
{
private:
    std::shared_ptr<SymbolTable> p_symbolTable;

    std::optional<Error> Consume(const Token &token, const Token &&expextedToken, int &index) const;
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

public:
    SyntaxAnalyzer(std::shared_ptr<SymbolTable> symbolTable);
    std::optional<Error> Execute();
};