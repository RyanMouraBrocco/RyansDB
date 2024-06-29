#pragma once

#include <memory>
#include <mutex>
#include "../Base/SyntaxParser.hpp"
#include "../LogicalOperationParser/LogicalOperationParser.hpp"

class SelectParser : public SyntaxParser
{
private:
    static std::shared_ptr<SelectParser> p_singleton;
    static std::mutex m_mutex;
    static std::shared_ptr<SelectParser> GetInstance();

    std::optional<Error> CheckSelectItems(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckFrom(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckTableName(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckJoin(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;

public:
    std::optional<Error> ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const override;
};