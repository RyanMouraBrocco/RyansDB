#include "ParserTree.hpp"

ParserToken::ParserToken(TokenDefinition tokenDefinition) : m_type(ParserType::Terminal), m_tokenDefintion(tokenDefinition)
{
}

ParserToken::ParserToken(NonTerminalToken nonTerminalToken) : m_type(ParserType::Non_Terminal), m_nonTerminalToken(nonTerminalToken)
{
}

ParserTreeNode::ParserTreeNode(std::shared_ptr<ParserTreeNode> father, ParserToken token) : p_father(father), m_token(token)
{
}

ParserTreeNode::ParserTreeNode(ParserToken token) : m_token(token)
{
}

void ParserTreeNode::AddChild(const std::shared_ptr<ParserTreeNode> child)
{
    p_children.push_back(child);
}

ParserTree::ParserTree()
{
}

void ParserTree::AddNodeInCurrentTier(TokenDefinition token)
{
    if (p_root == nullptr)
    {
        p_root = std::make_shared<ParserTreeNode>(ParserToken(token));
        m_nonTerminalLevel.push(p_root);
    }
    else
    {
        std::shared_ptr<ParserTreeNode> lastToken = m_nonTerminalLevel.top();
        std::shared_ptr<ParserTreeNode> child = std::make_shared<ParserTreeNode>(lastToken, ParserToken(token));
        lastToken->AddChild(child);
    }
}

void ParserTree::AddNodeInCurrentTier(NonTerminalToken token)
{
    if (p_root == nullptr)
    {
        p_root = std::make_shared<ParserTreeNode>(ParserToken(token));
        m_nonTerminalLevel.push(p_root);
    }
    else
    {
        std::shared_ptr<ParserTreeNode> lastToken = m_nonTerminalLevel.top();
        std::shared_ptr<ParserTreeNode> child = std::make_shared<ParserTreeNode>(lastToken, ParserToken(token));
        lastToken->AddChild(child);
        m_nonTerminalLevel.push(child);
    }
}

void ParserTree::TierUp()
{
    m_nonTerminalLevel.pop();
}
