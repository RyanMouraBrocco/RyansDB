#pragma once

#include <memory>
#include <mutex>
#include "../Base/SyntaxParser.hpp"
#include "../LogicalOperationParser/LogicalOperationParser.hpp"

class InsertParser : public SyntaxParser
{
private:
    static std::shared_ptr<InsertParser> p_singleton;
    static std::mutex m_mutex;

    std::optional<Error> CheckInsertStatement(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckInsertValues(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckInsertValue(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckInsertIdentifiers(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;

public:
    static std::shared_ptr<InsertParser> GetInstance();
    std::optional<Error> ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const override;
};