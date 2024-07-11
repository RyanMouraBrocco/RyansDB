#include "SemanticAnalyzer.hpp"

SemanticAnalyzer::SemanticAnalyzer(std::shared_ptr<SymbolTable> symbolTable) : p_symbolTable(symbolTable)
{
}

std::optional<Error> SemanticAnalyzer::Execute()
{
    auto logicalNodes = GetLogicalNodes();
    for (auto logicalNode : logicalNodes)
    {
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