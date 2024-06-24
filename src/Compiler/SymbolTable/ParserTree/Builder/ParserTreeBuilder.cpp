#include "ParserTreeBuilder.hpp"

ParserTreeBuider::ParserTreeBuider()
{
}

std::optional<std::shared_ptr<ParserTreeNode>> ParserTreeBuider::AddNodeAndGetChildIfExists(ParserToken token)
{
    if (p_root == nullptr)
    {
        p_root = std::make_shared<ParserTreeNode>(token);
        m_nonTerminalLevel.push(p_root);
        return std::nullopt;
    }

    std::shared_ptr<ParserTreeNode> lastToken = m_nonTerminalLevel.top();
    std::shared_ptr<ParserTreeNode> child = std::make_shared<ParserTreeNode>(lastToken, token);
    lastToken->AddChild(child);
    return child;
}

void ParserTreeBuider::AddNodeInCurrentTier(TokenDefinition token)
{
    AddNodeAndGetChildIfExists(ParserToken(token));
}

void ParserTreeBuider::AddNodeInCurrentTier(NonTerminalToken token)
{
    auto hasChildResult = AddNodeAndGetChildIfExists(ParserToken(token));
    if (hasChildResult.has_value())
        m_nonTerminalLevel.push(hasChildResult.value());
}

void ParserTreeBuider::TierUp()
{
    m_nonTerminalLevel.pop();
}