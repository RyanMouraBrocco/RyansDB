#pragma once

#include <mutex>
#include <memory>
#include <optional>
#include "../Base/SyntaxParser.hpp"

class UtilsParser : public SyntaxParser
{
private:
    static std::shared_ptr<UtilsParser> p_singleton;
    static std::mutex m_mutex;

public:
    static std::shared_ptr<UtilsParser> GetInstance();
    std::optional<Error> CheckFactorExpression(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckIdentifierAttribute(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckColumnDefinition(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckColumnType(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckTextType(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    std::optional<Error> CheckColumnContraints(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
    bool IsColumnConstraint(const std::vector<TokenDefinition> &tokens, const int &index) const;
    std::optional<Error> CheckColumnContraint(std::shared_ptr<SymbolTable> symbolTable, const std::vector<TokenDefinition> &tokens, int &index) const;
};