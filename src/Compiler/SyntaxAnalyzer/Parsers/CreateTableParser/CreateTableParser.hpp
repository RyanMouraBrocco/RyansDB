#pragma once

#include <memory>
#include <mutex>
#include "../Base/SyntaxParser.hpp"

class CreateTableParser : public SyntaxParser
{
private:
    static std::shared_ptr<CreateTableParser> p_singleton;
    static std::mutex m_mutex;

    std::optional<Error> CheckCreateTableStatement(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckColumnsDefinitions(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckColumnDefinition(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckColumnType(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckTextType(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckColumnContraints(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    bool IsColumnConstraint(const std::vector<TokenDefinition> &tokens, const int &index) const;
    std::optional<Error> CheckColumnContraint(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;

public:
    static std::shared_ptr<CreateTableParser> GetInstance();
    std::optional<Error> ParserIntoParserTree(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const override;
};