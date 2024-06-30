#pragma once

#include <mutex>
#include "../Base/CommandParser.hpp"
#include "../Utils/UtilsParser.hpp"

class LogicalOperationParser : public CommandParser
{
private:
    static std::shared_ptr<LogicalOperationParser> p_singleton;
    static std::mutex m_mutex;

    std::shared_ptr<UtilsParser> p_utilsParser = UtilsParser::GetInstance();

    std::optional<Error> CheckAndOperation(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckTokenExpression(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckComparisonExpression(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckCompareAction(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;

public:
    static std::shared_ptr<LogicalOperationParser> GetInstance();
    std::optional<Error> ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const override;
};