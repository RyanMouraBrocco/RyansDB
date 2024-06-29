#pragma once

#include <mutex>
#include "../Base/SyntaxParser.hpp"

class LogicalOperationParser : public SyntaxParser
{
    static std::shared_ptr<LogicalOperationParser> p_singleton;
    static std::mutex m_mutex;
    static std::shared_ptr<LogicalOperationParser> GetInstance();

    std::optional<Error> CheckAndOperation(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckTokenExpression(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckComparisonExpression(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckCompareAction(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckFactorExpression(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckIdentifierAttribute(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;

public:
    std::optional<Error> ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const override;
};