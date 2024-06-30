#pragma once

#include <memory>
#include <mutex>
#include "../Base/CommandParser.hpp"
#include "../LogicalOperationParser/LogicalOperationParser.hpp"

class DeleteParser : public CommandParser
{
private:
    static std::shared_ptr<DeleteParser> p_singleton;
    static std::mutex m_mutex;
    std::shared_ptr<LogicalOperationParser> p_logicalExpressionParser = LogicalOperationParser::GetInstance();

    std::optional<Error> CheckDeleteStatement(const std::vector<TokenDefinition> &tokens, int &index) const;

public:
    static std::shared_ptr<DeleteParser> GetInstance();
    std::optional<Error> ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const override;
};