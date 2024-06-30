#pragma once

#include <memory>
#include <mutex>
#include "../Base/CommandParser.hpp"
#include "../LogicalOperationParser/LogicalOperationParser.hpp"

class SelectParser : public CommandParser
{
private:
    static std::shared_ptr<SelectParser> p_singleton;
    static std::mutex m_mutex;
    std::shared_ptr<LogicalOperationParser> p_logicalExpressionParser = LogicalOperationParser::GetInstance();

    std::optional<Error> CheckSelectItems(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckFrom(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckTableName(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckJoin(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;

public:
    static std::shared_ptr<SelectParser> GetInstance();
    std::optional<Error> ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const override;
};