#pragma once

#include <memory>
#include <mutex>
#include "../Base/CommandParser.hpp"
#include "../LogicalOperationParser/LogicalOperationParser.hpp"
#include "../Utils/UtilsParser.hpp"

class UpdateParser : public CommandParser
{
private:
    static std::shared_ptr<UpdateParser> p_singleton;
    static std::mutex m_mutex;
    std::shared_ptr<LogicalOperationParser> p_logicalExpressionParser = LogicalOperationParser::GetInstance();
    std::shared_ptr<UtilsParser> p_utilsParser = UtilsParser::GetInstance();

    std::optional<Error> CheckUpdateSet(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;

public:
    static std::shared_ptr<UpdateParser> GetInstance();
    std::optional<Error> ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const override;
};