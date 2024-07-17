#pragma once

#include <vector>
#include <tuple>
#include <map>
#include <variant>
#include <memory>
#include <unordered_set>
#include "../../Error/error.hpp"
#include "./TokenDefinition/TokenDefinition.hpp"
#include "./ParserTree/Builder/ParserTreeBuilder.hpp"

class SymbolTable
{
private:
    std::shared_ptr<std::vector<TokenDefinition>> p_tokens;
    ParserTreeBuider m_parserTreeBuilder;

    static std::shared_ptr<std::vector<TokenDefinition>> m_reservedStatements;
    static std::map<char, Token> m_specialCharacterTokens;
    static std::unordered_set<Token> m_comparisionTokens;
    static std::unordered_set<Token> m_factorTokens;

public:
    SymbolTable();
    void AddToken(const std::string value, const Token key);
    std::shared_ptr<std::vector<TokenDefinition>> GetSortTokens() const;

    void AddNode(TokenDefinition token);
    void AddNode(NonTerminalToken nonTerminalToken);
    void TierUp();
    std::shared_ptr<ParserTreeNode> BuildTree();

    static bool IsSpecialCharacterToken(const char &value);
    static std::variant<Token, Error> GetSpecialCharacterToken(const char &value);
    static std::shared_ptr<std::vector<TokenDefinition>> GetReservedStatementsDefinitions();
    static bool IsComparisionToken(Token token);
    static bool IsFactorToken(Token token);
};