#include "ParserTreeNode.hpp"

ParserToken::ParserToken(TokenDefinition tokenDefinition) : m_type(ParserType::Terminal), m_tokenDefintion(tokenDefinition)
{
}

ParserToken::ParserToken(NonTerminalToken nonTerminalToken) : m_type(ParserType::Non_Terminal), m_nonTerminalToken(nonTerminalToken)
{
}

std::variant<TokenDefinition, NonTerminalToken> ParserToken::GetValue() const
{
    if (m_type == ParserType::Non_Terminal)
        return m_nonTerminalToken.value();

    return m_tokenDefintion.value();
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

std::vector<std::shared_ptr<ParserTreeNode>> ParserTreeNode::GetChildren() const
{
    return p_children;
}

ParserToken ParserTreeNode::GetToken() const
{
    return m_token;
}
