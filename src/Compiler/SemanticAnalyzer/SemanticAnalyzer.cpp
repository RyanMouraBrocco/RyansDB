#include "SemanticAnalyzer.hpp"

SemanticAnalyzer::SemanticAnalyzer(std::shared_ptr<SymbolTable> symbolTable) : p_symbolTable(symbolTable)
{
}

std::optional<Error> SemanticAnalyzer::Execute()
{
    auto logicalNodes = GetLogicalNodes();
    for (auto logicalNode : logicalNodes)
    {
        auto result = CheckLogicalNode(logicalNode);
        if (result.has_value())
            return result;
    }

    return std::nullopt;
}

std::vector<std::shared_ptr<ParserTreeNode>> SemanticAnalyzer::GetLogicalNodes()
{
    std::vector<std::shared_ptr<ParserTreeNode>> logicalNodes;
    std::queue<std::shared_ptr<ParserTreeNode>> nexts;
    nexts.push(p_symbolTable->BuildTree());

    while (!nexts.empty())
    {
        auto currentNode = nexts.front();
        nexts.pop();

        auto value = currentNode->GetToken().GetValue();
        if (std::holds_alternative<NonTerminalToken>(value) && std::get<NonTerminalToken>(value) == NonTerminalToken::LOGICAL_EXPRESSION)
            logicalNodes.push_back(currentNode);
        else
        {
            for (auto child : currentNode->GetChildren())
                nexts.push(child);
        }
    }

    return logicalNodes;
}

std::optional<Error> SemanticAnalyzer::CheckLogicalNode(std::shared_ptr<ParserTreeNode> logicalNode)
{
    std::stack<Wrapper> stack;

    stack.push(Wrapper(logicalNode, 0));

    while (!stack.empty())
    {
        auto currentNode = stack.top();
        auto value = currentNode.node->GetToken().GetValue();
        if (std::holds_alternative<TokenDefinition>(value))
            return Error(ErrorType::Unexpected, "Should not find a TokenDefiniton inside the logical node structure");

        auto children = currentNode.node->GetChildren();
        auto childrenCount = currentNode.node->GetChildren().size();
        if (childrenCount == 0)
            return Error(ErrorType::Unexpected, "All NonTerminalTokens should always have at least one child");

        auto nonTerminalToken = std::get<NonTerminalToken>(value);
        if (IsNotBooleanExpression(nonTerminalToken, children, childrenCount))
        {
            auto errorResult = ValidateBooleanFactorExpression(children[1]);
            if (errorResult.has_value())
                return errorResult;

            stack.pop();
        }
        else if (IsBooleanExpression(nonTerminalToken, children, childrenCount))
        {
            auto errorResult = ValidateBooleanFactorExpression(children[0]);
            if (errorResult.has_value())
                return errorResult;

            stack.pop();
        }
        else if (nonTerminalToken == NonTerminalToken::COMPARISON_EXPRESSION)
        {
            
        }
        else
        {
            if (currentNode.lastNodeIndex <= childrenCount)
            {
                stack.push(Wrapper(children[currentNode.lastNodeIndex], 0));
                currentNode.lastNodeIndex++;
            }
            else
                stack.pop();
        }
    }
}

bool SemanticAnalyzer::IsNotBooleanExpression(const NonTerminalToken &token, const std::vector<std::shared_ptr<ParserTreeNode>> &children, const int &childrenCount) const
{
    if (token != NonTerminalToken::TOKEN_EXPRESSION)
        return false;

    if (childrenCount != 2)
        return false;

    auto notToken = children[0]->GetToken().GetValue();
    if (!std::holds_alternative<TokenDefinition>(notToken) || std::get<TokenDefinition>(notToken).GetToken() != Token::NOT)
        return false;

    auto factorExpression = children[1]->GetToken().GetValue();
    if (!std::holds_alternative<NonTerminalToken>(factorExpression) || std::get<NonTerminalToken>(factorExpression) != NonTerminalToken::FACTOR_EXPRESSION)
        return false;

    return true;
}

bool SemanticAnalyzer::IsBooleanExpression(const NonTerminalToken &token, const std::vector<std::shared_ptr<ParserTreeNode>> &children, const int &childrenCount) const
{
    if (token != NonTerminalToken::TOKEN_EXPRESSION)
        return false;

    if (childrenCount != 1)
        return false;

    auto factorExpression = children[0]->GetToken().GetValue();
    if (!std::holds_alternative<NonTerminalToken>(factorExpression) || std::get<NonTerminalToken>(factorExpression) != NonTerminalToken::FACTOR_EXPRESSION)
        return false;

    return true;
}

std::optional<Error> SemanticAnalyzer::ValidateBooleanFactorExpression(const std::shared_ptr<ParserTreeNode> &factorExpressionNode) const
{
    auto token = factorExpressionNode->GetToken().GetValue();
    if (!std::holds_alternative<NonTerminalToken>(token) || std::get<NonTerminalToken>(token) != NonTerminalToken::IDENTIFIER_ATTRIBUTE)
        return Error(ErrorType::InvalidBooleanExpression, "Invalid bool expression");

    return std::nullopt;
}