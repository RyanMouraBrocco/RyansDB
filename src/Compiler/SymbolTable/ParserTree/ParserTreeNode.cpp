#include "ParserTreeNode.hpp"

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
