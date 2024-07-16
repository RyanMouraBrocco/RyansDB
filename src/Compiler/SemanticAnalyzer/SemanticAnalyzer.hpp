#pragma once

#include <memory>
#include <optional>
#include <queue>
#include <stack>
#include "../SymbolTable/SymbolTable.hpp"

struct Wrapper
{
    std::shared_ptr<ParserTreeNode> node;
    int lastNodeIndex;

    Wrapper(std::shared_ptr<ParserTreeNode> node, int lastNodexIndex) : node(node), lastNodeIndex(lastNodeIndex)
    {
    }
};

class SemanticAnalyzer
{
private:
    std::shared_ptr<SymbolTable> p_symbolTable;

    std::vector<std::shared_ptr<ParserTreeNode>> GetLogicalNodes();
    std::optional<Error> CheckLogicalNode(std::shared_ptr<ParserTreeNode> logicalNode);
    bool IsNotBooleanExpression(const NonTerminalToken &token, const std::vector<std::shared_ptr<ParserTreeNode>> &children, const int &childrenCount) const;
    bool IsBooleanExpression(const NonTerminalToken &token, const std::vector<std::shared_ptr<ParserTreeNode>> &children, const int &childrenCount) const;
    std::optional<Error> ValidateBooleanFactorExpression(const std::shared_ptr<ParserTreeNode> &factorExpressionNode) const;
    std::optional<Error> ValidateComparisionExpression(const std::shared_ptr<ParserTreeNode> &comparisionExpression) const;
    std::optional<Error> CompareLeftAndRightFactorExpressionOfAComparisionExpression(const std::shared_ptr<ParserTreeNode> &lefFactorExpression, const std::shared_ptr<ParserTreeNode> &rightFactorExpression) const;
    std::optional<Error> IsCompatibleTokens(const TokenDefinition &leftToken, const TokenDefinition &rightToken) const;
    bool IsNumericToken(const TokenDefinition &tokenDefinition) const;

public:
    SemanticAnalyzer(std::shared_ptr<SymbolTable> symbolTable);
    std::optional<Error> Execute();
};